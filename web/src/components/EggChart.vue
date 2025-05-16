<template>
    <Line v-if="chartData" :options="chartOptions" :data="chartData" />
    <v-skeleton-loader v-else type="image" />
</template>

<script>
import chroma from 'chroma-js';
import { Line } from 'vue-chartjs';
import {
    Chart as ChartJS,
    Tooltip,
    LineElement,
    CategoryScale,
    LinearScale,
    PointElement,
    Filler,
    Title,
} from 'chart.js';

ChartJS.register(Tooltip, LineElement, LinearScale, CategoryScale, PointElement, Filler, Title);

export default {
    name: 'EggChart',
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
            gradient: null,
            gradientLine: null,
            width: 0,
            height: 0,
            updateGradient: true,
            chartOptions: {
                responsive: true,
                plugins: {
                    tooltip: {
                        callbacks: {
                            label: ctx => {
                                return `${ctx.parsed.y} vajec`;
                            }
                        }
                    }
                },
                layout: {
                    padding: {
                        top: 10
                    }
                },
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
                }
            }
        };
    },
    computed: {
        chartData() {
            if (!this.arrayData) return null;
            return {
                labels: this.arrayLabels,
                datasets: [{
                    label: 'Vejce',   // Czech dataset label
                    data: this.arrayData,
                    fill: 'start',
                    radius: 0,
                    borderWidth: 5,
                    hitRadius: 10,
                    hoverRadius: 6,
                    pointBorderWidth: 0,
                    borderColor: ctx => this.createGradient(ctx, true),
                    backgroundColor: ctx => this.createGradient(ctx, false),
                    cubicInterpolationMode: 'monotone',
                }]
            };
        }
    },
    methods: {
        createGradient(context, forLine) {
            const { chart, chartArea, ctx } = context.chart;
            if (!chartArea) return;

            const w = chartArea.right - chartArea.left;
            const h = chartArea.bottom - chartArea.top;
            if (!this.gradient || !this.gradientLine ||
                this.updateGradient || this.width !== w || this.height !== h) {
                this.width = w;
                this.height = h;
                this.updateGradient = false;

                // light-blue → dark-blue over domain [0,10]
                const scale = chroma.scale(['lightblue', 'darkblue']).domain([0, 10]);

                // area fill gradient
                this.gradient = ctx.createLinearGradient(chartArea.left, 0, chartArea.right, 0);
                // line stroke gradient
                this.gradientLine = ctx.createLinearGradient(chartArea.left, 0, chartArea.right, 0);

                this.arrayData.forEach((val, i, arr) => {
                    const t = i / (arr.length - 1);
                    const c = scale(val).css();
                    this.gradient.addColorStop(t, chroma(c).alpha(0.3).css());
                    this.gradientLine.addColorStop(t, chroma(c).alpha(1).css());
                });
            }

            return forLine ? this.gradientLine : this.gradient;
        }
    },
    watch: {
        arrayData: {
            handler() {
                this.updateGradient = true;
            },
            deep: true,
            immediate: true
        }
    }
};
</script>
