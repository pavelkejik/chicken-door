<template>
    <v-card elevation="4">
        <div class="card-title-container ma-4">
            <div class="title-line"></div>
            <v-card-title class="headline text-center">Nastavení</v-card-title>
            <div class="title-line"></div>
        </div>

        <v-form v-model="isFormValid" @submit.prevent="submitData">
            <v-skeleton-loader :loading="loading" type="chip, ossein, heading, chip, ossein, heading, ossein, button">
                <v-card-text>
                    <v-card-title class="form-element">NTP</v-card-title>
                    <v-text-field class="form-element" clearable v-model="params.NTPServer" variant="outlined"
                        label="NTP server (hostname)"></v-text-field>

                    <v-card-title class="form-element">Lokace / Astro</v-card-title>
                    <v-select v-model="params.CasovePasmo" label="Lokace (nastaví časovou zónu + astro)"
                        :items="locations" item-value="value" item-text="title"
                        variant="outlined"></v-select>

                    <v-card-actions>
                        <v-btn v-if="hasChanges" class="mr-2" variant="outlined" color="primary" @click="resetSettings">
                            <v-icon left class="mr-2">{{ mdiCloseCircle }}</v-icon>
                            Vrátit změny</v-btn>
                        <v-btn v-if="hasChanges" variant="tonal" color="warning" type="submit">
                            <v-icon left class="mr-2">{{ mdiAlert }}</v-icon>
                            Uložit změny</v-btn>
                    </v-card-actions>

                </v-card-text>
            </v-skeleton-loader>


        </v-form>
    </v-card>
</template>
  
    
<script>

import baseParametersMixin from '@/mixins/baseParametersMixin';

const dataFields = {
    NTPServer: '',
    CasovePasmo: 0,
};

export default {
    mixins: [baseParametersMixin],
    name: 'DateTimeSettings',
    data() {
        return {
            params: dataFields,
            locations: [
                { title: 'Česko', value: 0 },
                { title: 'Slovensko', value: 1 },
                { title: 'Polsko', value: 2 },
                { title: 'Německo', value: 3 },
                { title: 'Rakousko', value: 4 },
                // Add more locations as needed
            ],
        };
    },
}
</script>
    
  