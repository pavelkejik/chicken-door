<template>
    <Line v-if="chartData" :options="chartOptions" :data="chartData" />
    <v-skeleton-loader v-else type="image"> </v-skeleton-loader>
</template>

<script>
import chroma from 'chroma-js';
import { Line } from 'vue-chartjs'
import {
    Chart as ChartJS,
    Tooltip,
    LineElement,
    CategoryScale,
    LinearScale,
    PointElement,
    Filler,
} from 'chart.js'
import annotationPlugin from 'chartjs-plugin-annotation';

ChartJS.register(Tooltip, LineElement, LinearScale, CategoryScale, PointElement, Filler, annotationPlugin)

export default {
    name: 'BatteryChart',
    components: { Line },
    props: {
        arrayData: {
            type: Array,
            required: true
        },
        arrayLabels: {
            type: Array,
            required: true
        },
        minVolt: {
            type: Number,
            required: true
        },
        maxVolt: {
            type: Number,
            required: true
        },
    },
    data() {
        return {
            gradientLine: null,
            width: 0,
            height: 0,
            gradient: null,
            updateGradient: true,
            chartOptions: {
                layout: {
                    padding: {
                        top: 10
                    }
                },
                responsive: true,
                scales: {
                    x: {
                        display: false,
                    },
                    y: {
                        display: false,
                        beginAtZero: true,
                        // afterDataLimits: axis => {
                        //     axis.max += 100;
                        // }
                    }
                },
                plugins: {
                    tooltip: {
                        callbacks: {
                            label: function (context) {
                                let label = context.dataset.label || '';
                                if (label) {
                                    label += ': ';
                                }
                                if (context.parsed.y !== null) {
                                    label += context.parsed.y + ' %';
                                }
                                return label;
                            }
                        }
                    },
                    annotation: {
                        annotations: {
                            threshold3: {
                                type: 'box',
                                xMin: 1,
                                xMax: 2,
                                yMin: 0,
                                yMax: 50,
                                backgroundColor: 'rgba(33, 200, 43,0.15)',
                                borderWidth: 0,
                            },
                            threshold4: {
                                type: 'line',
                                xMin: 1,
                                xMax: 2,
                                yMin: 50,
                                yMax: 50,
                                borderColor: 'rgba(33, 200, 43, 0.8)',
                                borderWidth: 8,
                                borderCapStyle: "round",
                            },

                        }
                    }
                },


            }
        }
    },
    computed:
    {
        chartData() {
            if (this.arrayData) {
                const labels = this.arrayLabels;
                const dataAsPercent = this.arrayData.map(mV => this.mVToPercent(mV & 0x7FFF));
                return {
                    labels,
                    datasets: [{
                        label: 'Baterie',
                        data: dataAsPercent,
                        borderColor: (ctx) => this.generateGradient(ctx, true),
                        radius: 0,
                        borderWidth: 5,
                        hitRadius: 10,
                        hoverRadius: 6,
                        pointBorderWidth: 0,
                        backgroundColor: (ctx) => this.generateGradient(ctx, false),
                        cubicInterpolationMode: 'monotone',
                        fill: 'start',
                    }]
                };
            }
            return null;
        },
        annotations() {
            let chargingAnnotations = {};
            let isCharging = false;
            let chargingStartIndex = null;
            const maxVal = this.mVToPercent(Math.max(...this.arrayData.map(dataPoint => dataPoint & 0x7FFF))) + 20;
            this.arrayData.forEach((dataPoint, index) => {
                // Check if highest bit is set (indicating charging)
                const charging = (dataPoint & 0x8000) !== 0;

                if (charging && !isCharging) {
                    // Start of charging interval
                    isCharging = true;
                    chargingStartIndex = index;
                } else if (!charging && isCharging) {
                    // End of charging interval, create annotation
                    const startLabel = chargingStartIndex;
                    const endLabel = index;
                    chargingAnnotations[`charging-${chargingStartIndex}-${index}`] = {
                        type: 'box',
                        xMin: startLabel - 0.5,
                        xMax: endLabel - 0.5,
                        yMin: 0,
                        yMax: maxVal,
                        backgroundColor: 'rgba(33, 200, 43,0.05)',
                        borderWidth: 0,
                    };

                    chargingAnnotations[`charging-line-${chargingStartIndex}-${index}`] = {
                        type: 'line',
                        xMin: startLabel - 0.5,
                        xMax: endLabel - 0.5,
                        yMin: maxVal,
                        yMax: maxVal,
                        borderColor: 'rgba(33, 200, 43, 0.8)',
                        borderWidth: 6,
                        borderCapStyle: "round",
                    };
                    isCharging = false;
                }
            });

            if (isCharging) {
                const startLabel = chargingStartIndex;
                const endLabel = this.arrayLabels.length;
                chargingAnnotations[`charging-${chargingStartIndex}-${endLabel}`] = {
                    type: 'box',
                    xMin: startLabel - 0.5,
                    xMax: endLabel - 0.5,
                    yMin: 0,
                    yMax: maxVal,
                    backgroundColor: 'rgba(33, 200, 43,0.05)',
                    borderWidth: 0,
                };

                chargingAnnotations[`charging-line-${chargingStartIndex}-${endLabel}`] = {
                    type: 'line',
                    xMin: startLabel - 0.5,
                    xMax: endLabel - 0.5,
                    yMin: maxVal,
                    yMax: maxVal,
                    borderColor: 'rgba(33, 200, 43, 0.8)',
                    borderWidth: 6,
                    borderCapStyle: "round",
                };
            }

            return chargingAnnotations;
        }
    },
    methods: {
        mVToPercent(mV) {
            const clampedVoltage = Math.max(this.minVolt, Math.min(this.maxVolt, mV));
            const percentage = ((clampedVoltage - this.minVolt) / (this.maxVolt - this.minVolt)) * 100;
            return Math.round(percentage);
        },

        generateGradient(context, line) {
            const chart = context.chart;
            const { ctx, chartArea } = chart;
            if (!chartArea) {
                return;
            }

            const chartWidth = chartArea.right - chartArea.left;
            const chartHeight = chartArea.bottom - chartArea.top;
            if (!this.gradient || !this.gradientLine || this.updateGradient || this.width !== chartWidth || this.height !== chartHeight) {
                this.gradient = ctx.createLinearGradient(chartArea.left, 0, chartArea.right, 0);
                this.gradientLine = ctx.createLinearGradient(chartArea.left, 0, chartArea.right, 0);
                this.width = chartWidth;
                this.height = chartHeight;
                this.updateGradient = false;
                const colorScale = chroma.scale(['red', 'yellow', 'green']).domain([this.minVolt, this.maxVolt]);
                this.arrayData.forEach((value, index, array) => {
                    const position = index / (array.length - 1);
                    const color = colorScale(value & 0x7FFF);
                    this.gradient.addColorStop(position, color.alpha(0.3).css());
                    this.gradientLine.addColorStop(position, color.alpha(1).css());
                });
            }
            if (line) {
                return this.gradientLine;
            }
            else {
                return this.gradient;
            }
        },
    },
    watch: {
        arrayData: {
            handler() {
                this.updateGradient = true;
            },
            deep: true,
            immediate: true,
        },
        annotations: {
            handler(newAnnotations) {
                this.chartOptions.plugins.annotation.annotations = newAnnotations;
            },
            deep: true,
            immediate: true,
        }
    },
}
</script>