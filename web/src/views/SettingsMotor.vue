<template>
  <v-card elevation="4">
      <div class="card-title-container ma-4">
          <div class="title-line"></div>
          <v-card-title class="headline text-center">Nastavení Pohonu</v-card-title>
          <div class="title-line"></div>
      </div>

      <v-form v-model="isFormValid" @submit.prevent="submitData">
          <v-skeleton-loader v-if="loading" type="chip, ossein, ossein, heading, heading, chip, ossein, ossein, heading, heading, chip, ossein, ossein, heading, heading, heading,">
        </v-skeleton-loader>

              <v-card-text v-else>



                  <v-card-title class="form-element">Proud</v-card-title>
                  <v-text-field class="form-element" v-model="params.MotorProudOtMax" :rules="[rules.required]" variant="outlined" type="number" :min="30" :max="1000" step="1"
                      label="Maximální proud otevírání (mA)"></v-text-field>

                      <v-text-field class="form-element" v-model="params.MotorProudOtMin" :rules="[rules.required]" variant="outlined" type="number" :min="30" :max="1000" step="1"
                      label="Maximální proud zavírání (mA)"></v-text-field>


                      <v-card-title class="form-element">Konstanty regulace polohy a rychlosti</v-card-title>
                      <v-text-field class="form-element" v-model="params.MaxRychlost" :rules="[rules.required]" variant="outlined" type="number" :min="30" :max="200" step="1"
                      label="Maximální rychlost pohybu (mm/s)"></v-text-field>

                      <v-text-field class="form-element" v-model="params.MinRychlost" :rules="[rules.required]" variant="outlined" type="number" :min="0" :max="100" step="1"
                      label="Minimální rychlost pohybu (mm/s)"></v-text-field>
                  <v-text-field class="form-element" v-model="params.PIDRychlostP" :rules="[rules.required]" variant="outlined" type="number" :min="0" :max="1000" step="1"
                      label="P konstanta pro rychlost"></v-text-field>

                      <v-text-field class="form-element" v-model="params.PIDRychlostI" :rules="[rules.required]" variant="outlined" type="number" :min="0" :max="1000" step="1"
                      label="I konstanta pro rychlost"></v-text-field>

                      <v-text-field class="form-element" v-model="params.PIDPolohaP" :rules="[rules.required]" variant="outlined" type="number" :min="0" :max="5000" step="1"
                      label="P konstanta pro polohu"></v-text-field>

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
  MaxRychlost: 70,
  MinRychlost: 20,
  MotorProudOtMax: 200,
  MotorProudOtMin: 100,
  PIDRychlostP: 50,
  PIDRychlostI: 25,
  PIDPolohaP: 1500,

};

export default {
  mixins: [baseParametersMixin],
  name: 'SettingsMotor',
  data() {
      return {
          params: dataFields,
      };
  },
}
</script>
  
