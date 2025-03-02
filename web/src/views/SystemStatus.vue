<template>
    <v-card elevation="4">
        <div class="card-title-container ma-4">
            <div class="title-line"></div>
            <v-card-title class="headline text-center">Stav Systému</v-card-title>
            <div class="title-line"></div>
        </div>

        <v-card-text>

            <v-skeleton-loader v-if="loading"
                type="avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider,button">
            </v-skeleton-loader>

            <div v-else>

                <v-list lines="two" item-props>

                    <v-list-item>
                        <template v-slot:prepend>
                            <v-avatar color="grey-darken-1">
                                <v-icon>{{ mdiWrench }}</v-icon>
                            </v-avatar>
                        </template>

                        <v-list-item-content>
                            <v-list-item-title class="text-wrap">Firmware (Verze / Datum kompilace)</v-list-item-title>
                            <v-list-item-subtitle>{{ FWText }}</v-list-item-subtitle>
                        </v-list-item-content>

                    </v-list-item>
                    <v-divider inset></v-divider>

                    <v-list-item>
                        <template v-slot:prepend>
                            <v-avatar color="grey-darken-1">
                                <v-icon>{{ mdiCellphoneLink }}</v-icon>
                            </v-avatar>
                        </template>

                        <v-list-item-content>
                            <v-list-item-title class="text-wrap">Verze SDK</v-list-item-title>
                            <v-list-item-subtitle>{{ params.SDKVersion }}</v-list-item-subtitle>
                        </v-list-item-content>

                    </v-list-item>
                    <v-divider inset></v-divider>

                    <v-list-item>
                        <template v-slot:prepend>
                            <v-avatar color="grey-darken-1">
                                <v-icon>{{ mdiTimer }}</v-icon>
                            </v-avatar>
                        </template>

                        <v-list-item-content>
                            <v-list-item-title class="text-wrap">System Uptime</v-list-item-title>
                            <v-list-item-subtitle>{{ formattedUptime }}</v-list-item-subtitle>
                        </v-list-item-content>

                    </v-list-item>
                    <v-divider inset></v-divider>

                    <v-list-item>
                        <template v-slot:prepend>
                            <v-avatar color="grey-darken-1">
                                <v-icon>{{ mdiChartLineVariant }}</v-icon>
                            </v-avatar>
                        </template>

                        <v-list-item-content>
                            <v-list-item-title class="text-wrap">Frekvence CPU</v-list-item-title>
                            <v-list-item-subtitle>{{ formattedCPUFreq }}</v-list-item-subtitle>
                        </v-list-item-content>
                    </v-list-item>
                    <v-divider inset></v-divider>


                    <v-list-item>
                        <template v-slot:prepend>
                            <v-avatar color="grey-darken-1">
                                <v-icon>{{ mdiMemory }}</v-icon>
                            </v-avatar>
                        </template>

                        <v-list-item-content>
                            <v-list-item-title class="text-wrap">Heap (Použito / Velikost)</v-list-item-title>
                            <v-list-item-subtitle>{{ formattedHeap }}</v-list-item-subtitle>
                        </v-list-item-content>

                    </v-list-item>
                    <v-divider inset></v-divider>

                    <v-list-item>
                        <template v-slot:prepend>
                            <v-avatar color="grey-darken-1">
                                <v-icon>{{ mdiApps }}</v-icon>
                            </v-avatar>
                        </template>

                        <v-list-item-content>
                            <v-list-item-title class="text-wrap">PSRAM (Použito / Velikost)</v-list-item-title>
                            <v-list-item-subtitle>{{ formattedPsram }}</v-list-item-subtitle>
                        </v-list-item-content>

                    </v-list-item>
                    <v-divider inset></v-divider>

                    <v-list-item>
                        <template v-slot:prepend>
                            <v-avatar color="grey-darken-1">
                                <v-icon>{{ mdiSd }}</v-icon>
                            </v-avatar>
                        </template>

                        <v-list-item-content>
                            <v-list-item-title class="text-wrap">Flash (Velikost / Rychlost)</v-list-item-title>
                            <v-list-item-subtitle>{{ formattedFlash }}</v-list-item-subtitle>
                        </v-list-item-content>

                    </v-list-item>
                    <v-divider inset></v-divider>

                    <v-list-item>
                        <template v-slot:prepend>
                            <v-avatar color="grey-darken-1">
                                <v-icon>{{ mdiApplicationBrackets }}</v-icon>
                            </v-avatar>
                        </template>

                        <v-list-item-content>
                            <v-list-item-title class="text-wrap">Aplikace (Použito / Velikost oddílu)</v-list-item-title>
                            <v-list-item-subtitle>{{ formattedAPP }}</v-list-item-subtitle>
                        </v-list-item-content>

                    </v-list-item>
                    <v-divider inset></v-divider>

                    <v-list-item>
                        <template v-slot:prepend>
                            <v-avatar color="grey-darken-1">
                                <v-icon>{{ mdiFolder }}</v-icon>
                            </v-avatar>
                        </template>

                        <v-list-item-content>
                            <v-list-item-title class="text-wrap">Souborový systém (Použito / Velikost
                                oddílu)</v-list-item-title>
                            <v-list-item-subtitle>{{ formattedFS }}</v-list-item-subtitle>
                        </v-list-item-content>

                    </v-list-item>
                    <v-divider inset></v-divider>

                </v-list>


                <v-card-actions>

                    <v-btn @click="restartDevice" variant="outlined" color="red">
                        <v-icon left class="mr-2">{{ mdiPower }}</v-icon>
                        Restart</v-btn>
                </v-card-actions>

            </div>

        </v-card-text>

    </v-card>
</template>
  
<script>
import baseParametersMixin from '@/mixins/baseParametersMixin';
import axios from 'axios';

const dataFields = {
    MainRev: 0,
    CompDate: "",
    SDKVersion: "",
    HeapSize: 0,
    HeapUsed: 0,
    FlashSize: 0,
    FlashSpeed: 0,
    CPUFreq: 0,
    PsramSize: 0,
    PsramUsed: 0,
    APPSize: 0,
    APPUsed: 0,
    FSSize: 0,
    FSUsed: 0,
    Uptime: 0,
};


export default {
    mixins: [baseParametersMixin],
    name: 'SystemStatus',
    data: () => ({
        params: dataFields,
        fetchPeriod: 5000,
    }),
    computed: {
        FWText() {
            return `${this.params.MainRev} / ${this.params.CompDate}`;
        }
        ,
        formattedUptime() {
            const seconds = this.params.Uptime % 60;
            const minutes = Math.floor(this.params.Uptime / 60) % 60;
            const hours = Math.floor(this.params.Uptime / 3600) % 24;
            const days = Math.floor(this.params.Uptime / 86400);

            return `${days}d ${hours.toString().padStart(2, '0')}:${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;
        },
        formattedCPUFreq() {
            return `${this.params.CPUFreq} MHz`;
        },
        formattedHeap() {
            return `${this.params.HeapUsed} KB / ${this.params.HeapSize} KB`;
        },
        formattedPsram() {
            return `${this.params.PsramUsed} KB / ${this.params.PsramSize} KB`;
        },
        formattedFlash() {
            return `${this.params.FlashSize} KB / ${this.params.FlashSpeed} MHz`;
        },
        formattedAPP() {
            return `${this.params.APPUsed} KB / ${this.params.APPSize} KB`;
        },
        formattedFS() {
            return `${this.params.FSUsed} KB / ${this.params.FSSize} KB`;
        },
    },
    methods: {
        restartDevice() {
            axios.get('/api/restart')
                .then(response => {
                    this.$toast.success('Zařízení se nyní restartuje.');
                })
                .catch(error => {
                    this.$toast.error('Došlo k chybě při zpracování požadavku. Zkuste to prosím znovu později.');
                })
        },
    }
};
</script>