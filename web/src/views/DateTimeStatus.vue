<template>
    <v-card elevation="4">
        <div class="card-title-container ma-4">
            <div class="title-line"></div>
            <v-card-title class="headline text-center">Datum a čas</v-card-title>
            <div class="title-line"></div>
        </div>

        <v-card-text>

            <v-skeleton-loader v-if="loading"
                type="chip, ossein, ossein, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, chip, ossein, ossein, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider">
            </v-skeleton-loader>

            <v-list v-else lines="two" item-props>

                <v-list-subheader>NTP</v-list-subheader>

                <v-list-item>
                    <template v-slot:prepend>
                        <v-avatar :color="params.NTPSyncCas ? 'green':'red'">
                            <v-icon>{{ mdiUpdate }}</v-icon>
                        </v-avatar>
                    </template>

                    <v-list-item-content>
                        <v-list-item-title class="text-wrap">Poslední synchronizace s NTP</v-list-item-title>
                        <v-list-item-subtitle>{{ params.NTPSyncCas ? params.NTPSyncCas : 'Čas není synchronizován' }}</v-list-item-subtitle>
                    </v-list-item-content>

                </v-list-item>
                <v-divider inset></v-divider>

                <v-list-item>
                    <template v-slot:prepend>
                        <v-avatar color="grey-darken-1">
                            <v-icon>{{ mdiServer }}</v-icon>
                        </v-avatar>
                    </template>

                    <v-list-item-content>
                        <v-list-item-title class="text-wrap">NTP server</v-list-item-title>
                        <v-list-item-subtitle>{{ params.NTPServer }}</v-list-item-subtitle>
                    </v-list-item-content>

                </v-list-item>
                <v-divider inset></v-divider>

                <v-divider class="mt-9 mb-5"></v-divider>
                <v-list-subheader>Datum a čas</v-list-subheader>

                <v-list-item>
                    <template v-slot:prepend>
                        <v-avatar color="grey-darken-1">
                            <v-icon>{{ mdiTimer }}</v-icon>
                        </v-avatar>
                    </template>

                    <v-list-item-content>
                        <v-list-item-title class="text-wrap">Aktuální čas</v-list-item-title>
                        <v-list-item-subtitle>{{ params.AktualniCas }}</v-list-item-subtitle>
                    </v-list-item-content>

                </v-list-item>
                <v-divider inset></v-divider>

                <v-list-item>
                    <template v-slot:prepend>
                        <v-avatar color="grey-darken-1">
                            <v-icon>{{ mdiWeatherSunsetUp }}</v-icon>
                        </v-avatar>
                    </template>

                    <v-list-item-content>
                        <v-list-item-title class="text-wrap">Východ slunce</v-list-item-title>
                        <v-list-item-subtitle>{{ params.AstroVychod }}</v-list-item-subtitle>
                    </v-list-item-content>

                </v-list-item>
                <v-divider inset></v-divider>

                <v-list-item>
                    <template v-slot:prepend>
                        <v-avatar color="grey-darken-1">
                            <v-icon>{{ mdiWeatherSunsetDown }}</v-icon>
                        </v-avatar>
                    </template>

                    <v-list-item-content>
                        <v-list-item-title class="text-wrap">Západ slunce</v-list-item-title>
                        <v-list-item-subtitle>{{ params.AstroZapad }}</v-list-item-subtitle>
                    </v-list-item-content>

                </v-list-item>
                <v-divider inset></v-divider>

            </v-list>

        </v-card-text>

    </v-card>
</template>
  
<script>
import baseParametersMixin from '@/mixins/baseParametersMixin';

const dataFields = {
    NTPServer: "",
    AstroVychod: "",
    AstroZapad: "",
    NTPSyncCas: "",
    AktualniCas: "",
};


export default {
    mixins: [baseParametersMixin],
    name: 'DateTimeStatus',
    data: () => ({
        params: dataFields,
        fetchPeriod: 5000,
    }),
};
</script>