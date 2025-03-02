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
    name: 'FoodChart',
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
        treshold: {
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

        }
    },
    computed: {
        chartOptions() {
            return {
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
                        beginAtZero: true,
                        display: false,
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
                            threshold1: {
                                type: 'line',
                                yMin: this.treshold,
                                yMax: this.treshold,
                                borderColor: 'rgb(255, 224, 0)',
                                borderWidth: 3,
                                borderDash: [5, 5],
                                label: {
                                    content: 'Threshold 1',
                                    enabled: true,
                                    position: 'start'
                                }
                            },
                        }
                    }
                },
            }
        },
        chartData() {
            if (this.arrayData) {
                const labels = this.arrayLabels;
                return {
                    labels,
                    datasets: [{
                        label: 'Stav krmiva',
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
                const minTemp = 0;
                const maxTemp = 100;
                const colorScale = chroma.scale(['lightblue', 'orange']).domain([minTemp, maxTemp]);
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
  