<template>
    <v-row>
        <v-col cols="12">
            <v-card variant="tonal" color="blue-grey" elevation="2">
                <v-card-title class="d-flex justify-space-between align-center">
                    <span class="text-medium-emphasis">Stav</span>
                    <v-chip v-if="!loading" color="primary" :value="batteryLevel">
                        <v-icon left class="mr-2"> {{ batteryIcon }}</v-icon>
                        {{ batteryLevel }}%
                    </v-chip>
                </v-card-title>

                <v-skeleton-loader class="ml-2 mt-2" v-if="loading" type="button"> </v-skeleton-loader>
                <v-card-text v-else class="text-h5 text-left ml-2 mt-2">
                    <v-sheet>
                        {{ deviceState }}
                    </v-sheet>

                </v-card-text>
                <v-skeleton-loader class="ml-2" v-if="loading" type="button, button"> </v-skeleton-loader>
                <v-card-actions v-else>
                    <v-btn class="ml-4 mr-4" color="green" variant="tonal" @click="feederCommand(1)">
                        <v-icon left class="mr-1">{{ mdiTrayArrowDown }}</v-icon>
                        Otevřít
                    </v-btn>
                    <v-btn color="red" variant="tonal" @click="feederCommand(2)">
                        <v-icon left class="mr-1">{{ mdiTrayArrowUp }}</v-icon>
                        Zavřít
                    </v-btn>
                </v-card-actions>
                <v-divider class="mt-4"></v-divider>

                <v-skeleton-loader class="mb-10 mt-6" v-if="loading" type="list-item, list-item"> </v-skeleton-loader>
                <v-card-text v-else>
                    <v-row no-gutters>
                        <v-col cols="8">
                            <v-sheet class="pa-2 ma-2">
                                <v-icon size="28" class="mr-4">{{ mdiWindowShutterOpen }}</v-icon>
                                Poslední otevření v:
                            </v-sheet>

                        </v-col>
                        <v-col>
                            <v-sheet class="pa-2 ma-2 text-right">
                                {{ params.PosledniCasOtevreni }}
                            </v-sheet>
                        </v-col>
                    </v-row>

                    <v-row no-gutters>
                        <v-col cols="8">
                            <v-sheet class="pa-2 ma-2">
                                <v-icon size="28" class="mr-4">{{ mdiWindowShutter }}</v-icon>
                                Poslední zavření v:
                            </v-sheet>
                        </v-col>
                        <v-col>
                            <v-sheet class="pa-2 ma-2 text-right">
                                {{ params.PosledniCasZavreni }}
                            </v-sheet>
                        </v-col>
                    </v-row>


                </v-card-text>
            </v-card>
        </v-col>

        <v-col cols="12">

            <v-card variant="tonal" color="blue-grey" elevation="4">
                <v-card-title class="d-flex justify-space-between align-center">
                    <span class="text-medium-emphasis">Stav krmiva</span>
                    <v-icon color="primary">{{ mdiWeight }}</v-icon>
                </v-card-title>

                <v-skeleton-loader v-if="loading" type="button"> </v-skeleton-loader>
                <v-card-text v-else class="text-h4 text-left mt-4">
                    {{ params.AktualniVaha_proc.curr }} %
                </v-card-text>

                <food-chart :array-data="foodChartData" :array-labels="foodChartLabels"
                    :treshold="params.UrovenDoplneni_proc" style="max-height: 200px;"></food-chart>

                <v-card-text>

                    <v-card-actions class="d-flex flex-wrap justify-space-between mt-2">

                        <v-btn class="mb-2" color="primary" variant="tonal" @click="tareCommand(true)">
                            <v-icon left class="mr-1">{{ mdiTray }}</v-icon>
                            Krmítko je prázdné
                        </v-btn>
                        <v-btn class="mb-2" color="primary" variant="tonal" @click="tareCommand(false)">
                            <v-icon left class="mr-1">{{ mdiTrayFull }}</v-icon>
                            Krmítko je plné
                        </v-btn>
                    </v-card-actions>

                    <div class="text-subtitle-1 text-medium-emphasis ml-2">Úroveň pro doplnění (%)</div>
                    <v-slider v-model="params.UrovenDoplneni_proc" color="primary" :prepend-icon="`${mdiWeight}`"
                        @end="sendFilling" :max="100" :min="0" :step="1" thumb-label>
                        <template v-slot:append>
                            <v-text-field hide-details density="compact" style="width: 80px" disabled=""
                                v-model="params.UrovenDoplneni_proc" variant="outlined" type="number" :max="100"
                                :min="0" :step="1"></v-text-field>
                        </template>
                    </v-slider>
                </v-card-text>
            </v-card>
        </v-col>

        <v-col cols="12">

            <v-card variant="tonal" color="blue-grey" elevation="4">
                <v-card-title class="d-flex justify-space-between align-center">
                    <span class="text-medium-emphasis">Denní automatika</span>
                    <v-icon color="primary">{{ mdiWindowShutterAuto }}</v-icon>
                </v-card-title>

                <v-skeleton-loader class="mb-10 mt-6" v-if="loading" type="list-item, list-item"> </v-skeleton-loader>
                <v-card-text v-else>
                    <v-row v-if="params.AutomatikaOtevreni" no-gutters>
                        <v-col cols="8">
                            <v-sheet class="pa-2 ma-2">
                                <v-icon size="28" class="mr-4">{{ mdiSunClock }}</v-icon>
                                Otevření v:
                            </v-sheet>
                        </v-col>
                        <v-col>
                            <v-sheet class="pa-2 ma-2 text-right">
                                {{ params.CasOtevreni }}
                            </v-sheet>
                        </v-col>
                    </v-row>

                    <v-row v-if="params.AutomatikaZavreni" no-gutters>
                        <v-col cols="8">
                            <v-sheet class="pa-2 ma-2">
                                <v-icon size="28" class="mr-4">{{ mdiSunClock }}</v-icon>
                                Zavření v:
                            </v-sheet>
                        </v-col>
                        <v-col>
                            <v-sheet class="pa-2 ma-2 text-right">
                                {{ params.CasZavreni }}
                            </v-sheet>
                        </v-col>
                    </v-row>

                    <v-row v-if="!params.AutomatikaZavreni && !params.AutomatikaOtevreni" no-gutters>
                        <v-col class="text-center">
                            <v-icon size="28" color="primary" class="mr-4">{{ mdiCloseCircle }}</v-icon>


                            <span class="text-subtitle-1">Automatika není aktivní</span>
                        </v-col>

                    </v-row>
                </v-card-text>
            </v-card>
        </v-col>

        <v-col cols="12">
            <v-card variant="tonal" color="blue-grey" elevation="4">

                <v-card-title class="d-flex justify-space-between align-center">
                    <span class="text-medium-emphasis">Stav baterie</span>
                    <v-icon color="primary">{{ batteryIcon }}</v-icon>
                </v-card-title>

                <v-skeleton-loader v-if="loading" type="button"> </v-skeleton-loader>
                <v-card-text v-else class="text-h4 text-left mt-4">
                    {{ batteryLevel }} %<v-icon v-if="batteryCharging" size="35" color="green">{{ mdiLightningBolt
                        }}</v-icon>

                </v-card-text>

                <battery-chart :array-data="batChartData" :array-labels="batChartLabels" :min-volt="minVolt"
                    :max-volt="maxVolt" style="max-height: 200px;"></battery-chart>

            </v-card>
        </v-col>

    </v-row>
</template>

<script>
import axios from 'axios';
import {
    mdiTrayArrowUp,
    mdiTrayArrowDown,
    mdiBattery10,
    mdiBattery50,
    mdiBattery70,
    mdiBattery,
    mdiWindowShutterOpen,
    mdiWindowShutter,
    mdiWindowShutterAuto,
    mdiSunClock,
    mdiWeight,
    mdiCloseCircle,
    mdiTrayFull,
    mdiTray,
    mdiLightningBolt,
} from '@mdi/js'
import BatteryChart from '@/components/BatteryChart.vue';
import FoodChart from '@/components/FoodChart.vue';

import { deviceEnums } from '@/utils/deviceEnums';

const dataFields = {
    StavKrmitka: 0,
    AktualniVaha_proc: { curr: 50 },
    PosledniCasOtevreni: "--:--",
    PosledniCasZavreni: "--:--",
    CasOtevreni: "--:--",
    CasZavreni: "--:--",
    NapetiBaterie_mV: { curr: 3500, vals: [-28690, 36268, 36368, 3500, 3500, 36368, 3500, 3500, 3600, 3500, 3500, 36368], mins: 5 },
    UrovenDoplneni_proc: 20,
    AutomatikaOtevreni: false,
    AutomatikaZavreni: false,
};

export default {
    props: {
        deviceId: {
            type: Number,
            required: true
        }
    },
    components: {
        BatteryChart, FoodChart
    },
    data: () => ({
        mdiTrayArrowUp,
        mdiTrayArrowDown,
        mdiBattery10,
        mdiBattery50,
        mdiBattery70,
        mdiBattery,
        mdiWindowShutterOpen,
        mdiWindowShutter,
        mdiWindowShutterAuto,
        mdiSunClock,
        mdiWeight,
        mdiCloseCircle,
        mdiTrayFull,
        mdiTray,
        mdiLightningBolt,
        loading: true,
        params: dataFields,
        minVolt: 3100,
        maxVolt: 4130,
    }),
    created() {
        this.loading = true;
    },
    computed: {
        deviceState() {
            const feederEnums = deviceEnums[1].stateFlags;
            const stavKrmitkaValue = this.params.StavKrmitka;
            const enumItem = feederEnums.find(enumItem => enumItem.value === stavKrmitkaValue);
            return enumItem ? enumItem.text : 'Neznámý stav';
        },
        batteryLevel() {
            return this.mVToPercent(this.params.NapetiBaterie_mV.curr & 0x7FFF);
        },
        batteryCharging() {
            return (this.params.NapetiBaterie_mV.curr & 0x8000) != 0;
        },
        batteryIcon() {
            if (this.batteryLevel <= 10) {
                return this.mdiBattery10;
            } else if (this.batteryLevel <= 50) {
                return this.mdiBattery50;
            } else if (this.batteryLevel <= 85) {
                return this.mdiBattery70;
            } else {
                return this.mdiBattery;
            }
        },
        batChartData() {
            return this.params.NapetiBaterie_mV.vals;
        },
        batChartLabels() {
            if (this.params.NapetiBaterie_mV.vals) {
                return this.generateTimeLabels(this.params.NapetiBaterie_mV.vals.length, this.params.NapetiBaterie_mV.mins * 60 * 1000);
            }
            return null;
        },
        foodChartData() {
            return this.params.AktualniVaha_proc.vals;
        },
        foodChartLabels() {
            if (this.params.AktualniVaha_proc.vals) {
                return this.generateTimeLabels(this.params.AktualniVaha_proc.vals.length, this.params.AktualniVaha_proc.mins * 60 * 1000);
            }
            return null;
        }
    },
    methods: {
        generateTimeLabels(dataLength, step) {
            const labels = [];
            const now = new Date();
            for (let i = 0; i < dataLength; i++) {
                const time = new Date(now.getTime() - i * step);
                labels.push(time.toLocaleTimeString('cs-CZ', { hour: '2-digit', minute: '2-digit' }));
            }
            return labels;
        },
        mVToPercent(mV) {
            const clampedVoltage = Math.max(this.minVolt, Math.min(this.maxVolt, mV));
            const percentage = ((clampedVoltage - this.minVolt) / (this.maxVolt - this.minVolt)) * 100;
            return Math.round(percentage);
        },
        openFeeder() {
            this.$toast.success('Krmítko se otevře při příštím probuzení.');
        },
        closeFeeder() {
            this.$toast.success('Krmítko se zavře při příštím probuzení.');
        },
        fetchData(error_notify = true) {
            let queryParams = new URLSearchParams();

            for (const key in this.params) {
                if (this.params.hasOwnProperty(key)) {
                    queryParams.append(key, '');
                }
            }
            queryParams.append('id', this.deviceId);
            let queryString = queryParams.toString();
            axios.get(`/api/get_device_params?${queryString}`)
                .then(response => {
                    if (response.headers['content-type'].includes('application/json')) {
                        this.loading = false;
                        this.params = response.data;
                    } else {
                        throw new Error('Invalid response type');
                    }
                })
                .catch(error => {
                    console.error('Error loading parameters:', error);
                    if (error_notify) {
                        this.$toast.error('Došlo k chybě při načítání parametrů. Zkuste stránku obnovit.');
                    }
                });
        },


        feederCommand(command) {
            const payload = {
                id: this.deviceId,
                parameters: [
                    {
                        name: "ManualniOvladani",
                        val: command,
                    },
                ],
            };
            axios.post('/api/set_device_params', payload, {
                headers: {
                    'Content-Type': 'application/json',
                },
            })
                .then(response => {
                    if (command == 1) {
                        this.$toast.success('Krmítko se otevře při příštím probuzení.');
                    }
                    else {
                        this.$toast.success('Krmítko se zavře při příštím probuzení.');

                    }
                    this.fetchData();
                })
                .catch(error => {
                    console.error('Error updating parameter(s):', error);
                    this.$toast.error('Došlo k chybě při odesílání požadavku. Zkuste to prosím znovu později.');
                });
        },

        tareCommand(empty = true) {
            const payload = {
                id: this.deviceId,
                parameters: [
                    {
                        name: empty ? "KalibracePrazdne" : "KalibracePlne",
                        val: 2,
                    },
                ],
            };
            axios.post('/api/set_device_params', payload, {
                headers: {
                    'Content-Type': 'application/json',
                },
            })
                .then(response => {
                    this.$toast.success('Kalibrace proběhne při příštím probuzení.');
                    this.fetchData();
                })
                .catch(error => {
                    console.error('Error updating parameter(s):', error);
                    this.$toast.error('Došlo k chybě při odesílání požadavku. Zkuste to prosím znovu později.');
                });
        },

        sendFilling(newValue) {
            const payload = {
                id: this.deviceId,
                parameters: [
                    {
                        name: "UrovenDoplneni_proc",
                        val: newValue,
                    },
                ],
            };
            axios.post('/api/set_device_params', payload, {
                headers: {
                    'Content-Type': 'application/json',
                },
            })
                .then(response => {
                    // this.$toast.success('Kalibrace proběhne při příštím probuzení.');
                    // this.fetchData();
                })
                .catch(error => {
                    console.error('Error updating parameter(s):', error);
                    this.$toast.error('Došlo k chybě při odesílání požadavku. Zkuste to prosím znovu později.');
                });
        },
    },
};
</script>