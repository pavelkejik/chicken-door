<template>
    <v-card elevation="4">
        <div class="card-title-container ma-4">
            <div class="title-line"></div>
            <v-card-title class="headline text-center">Automatika</v-card-title>
            <div class="title-line"></div>
        </div>

        <v-form v-model="isFormValid" @submit.prevent="submitData">
            <v-skeleton-loader v-if="loading"
                type="chip, ossein, ossein, button, ossein, heading, button, ossein, ossein, heading, ossein, chip, ossein, ossein, button, ossein, heading, button, ossein, ossein, heading, ossein,">
            </v-skeleton-loader>

            <v-card-text v-else>
                <v-card-title class="form-element">Otevírání</v-card-title>
                <v-checkbox color="primary" v-model="params.OtevreniSvetlem"
                    label="Východ slunce podle světla"></v-checkbox>
                <div v-if="params.OtevreniSvetlem">
                    <div class="text-subtitle-1 text-medium-emphasis ml-2">Úroveň osvětlení pro den (%)</div>
                    <v-slider v-model="params.OsvetleniDen" color="primary" :prepend-icon="`${mdiBrightness6}`" :min="0"
                        :max="100" :step="1" thumb-label>
                        <template v-slot:append>
                            <v-text-field hide-details density="compact" style="width: 80px" v-model="params.OsvetleniDen"
                                :rules="[rules.required]" variant="outlined" type="number"
                                :min="params.ZavreniSvetlem ? params.OsvetleniNoc : 0" :max="100" :step="1"></v-text-field>
                        </template>
                    </v-slider>
                </div>

                <v-checkbox color="primary" v-model="params.OtevreniVychodem"
                    label="Astronomický východ slunce"></v-checkbox>


                <div class="text-subtitle-1 text-medium-emphasis ml-2">Zpoždění otevření po rozednění (minut)</div>
                <v-slider v-model="params.ZpozdeniOtevreni" color="primary" :prepend-icon="`${mdiClockPlusOutline}`"
                    :max="180" :min="0" :step="1" thumb-label>
                    <template v-slot:append>
                        <v-text-field hide-details density="compact" style="width: 80px" v-model="params.ZpozdeniOtevreni"
                            variant="outlined" type="number" :max="180" :min="0" :step="1"></v-text-field>
                    </template>
                </v-slider>


                <v-divider class="mb-4"></v-divider>
                <v-card-title class="form-element">Zavírání</v-card-title>
                <v-checkbox color="primary" v-model="params.ZavreniSvetlem" label="Západ slunce podle světla"></v-checkbox>

                <div v-if="params.ZavreniSvetlem">
                    <div class="text-subtitle-1 text-medium-emphasis ml-2">Úroveň osvětlení pro noc (%)</div>
                    <v-slider v-model="params.OsvetleniNoc" color="primary" :prepend-icon="`${mdiBrightness6}`" :max="100"
                        :min="0" :step="1" thumb-label>
                        <template v-slot:append>
                            <v-text-field hide-details density="compact" style="width: 80px" v-model="params.OsvetleniNoc"
                                :rules="[rules.required]" variant="outlined" type="number"
                                :max="params.OtevreniSvetlem ? params.OsvetleniDen : 100" :min="0" :step="1"></v-text-field>
                        </template>
                    </v-slider>
                </div>


                <v-checkbox color="primary" v-model="params.ZavreniZapadem" label="Astronomický západ slunce"></v-checkbox>

                <div class="text-subtitle-1 text-medium-emphasis ml-2">Zpoždění zavření po setmění (minut)</div>
                <v-slider v-model="params.ZpozdeniZavreni" color="primary" :prepend-icon="`${mdiClockPlusOutline}`"
                    :max="180" :min="0" :step="1" thumb-label>
                    <template v-slot:append>
                        <v-text-field hide-details density="compact" style="width: 80px" v-model="params.ZpozdeniZavreni"
                            variant="outlined" type="number" :max="180" :min="0" :step="1"></v-text-field>
                    </template>
                </v-slider>

                <v-card-actions>
                    <v-btn v-if="hasChanges" class="mr-2" variant="outlined" color="primary" @click="resetSettings">
                        <v-icon left class="mr-2">{{ mdiCloseCircle }}</v-icon>
                        Vrátit změny</v-btn>
                    <v-btn v-if="hasChanges" variant="tonal" color="warning" type="submit">
                        <v-icon left class="mr-2">{{ mdiAlert }}</v-icon>
                        Uložit změny</v-btn>
                </v-card-actions>

            </v-card-text>


        </v-form>
    </v-card>
</template>

  
<script>

import baseParametersMixin from '@/mixins/baseParametersMixin';

const dataFields = {
    OtevreniSvetlem: false,
    OtevreniVychodem: false,
    ZavreniSvetlem: false,
    ZavreniZapadem: false,
    ZpozdeniOtevreni: 5,
    ZpozdeniZavreni: 5,
    OsvetleniDen: 5,
    OsvetleniNoc: 20,
};

export default {
    mixins: [baseParametersMixin],
    name: 'SettingsAuto',
    data() {
        return {
            params: dataFields,
        };
    },
}
</script>
  
