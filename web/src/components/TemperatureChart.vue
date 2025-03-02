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

ChartJS.register(Tooltip, LineElement, LinearScale, CategoryScale, PointElement, Filler)

export default {
    name: 'TemperatureChart',
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
                        afterDataLimits: axis => {
                            axis.max += 3;
                            axis.min -= 3;
                        }
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
                                    label += context.parsed.y + '°C';
                                }
                                return label;
                            }
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
                return {
                    labels,
                    datasets: [{
                        label: 'Teplota',
                        data: this.arrayData,
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
        }
    },
    methods: {
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
                const minTemp = -20;
                const maxTemp = 40;
                const colorScale = chroma.scale(['blue', 'lightblue', 'yellow', 'red']).domain([minTemp, maxTemp]);
                this.arrayData.forEach((value, index, array) => {
                    const position = index / (array.length - 1);
                    const color = colorScale(value);
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
        }
    },
}
</script>