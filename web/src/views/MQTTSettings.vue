<template>
  <v-card elevation="4">
    <div class="card-title-container ma-4">
      <div class="title-line"></div>
      <v-card-title class="headline text-center">Nastavení MQTT</v-card-title>
      <div class="title-line"></div>
    </div>

    <v-form v-model="isFormValid" @submit.prevent="submitData">
      <v-skeleton-loader :loading="loading" type="chip, ossein, heading, heading, heading, heading, heading,">
        <v-card-text>
          <v-checkbox color="primary" v-model="params.MQTTOn" label="Povolit MQTT"></v-checkbox>


          <div v-if="params.MQTTOn">
            <v-text-field class="form-element" v-model="params.BrokerAdresa" variant="outlined"
              :rules="[rules.required, rules.ipAddress]" label="IP adresa brokeru"></v-text-field>
            <v-text-field class="form-element" v-model="params.BrokerPort" type="number" variant="outlined"
              :rules="[rules.required]" label="Port brokeru"></v-text-field>
            <v-text-field class="form-element" v-model="params.MQTTbase" variant="outlined" :rules="[rules.required]"
              label="Klient ID / Base topic"></v-text-field>
            <v-text-field class="form-element" v-model="params.MQTTJmeno" variant="outlined"
              label="Jméno"></v-text-field>
            <v-text-field class="form-element" v-model="params.MQTTHeslo" type="password" variant="outlined"
              label="Heslo"></v-text-field>

            <div class="text-subtitle-1 text-medium-emphasis ml-2">Perioda aktualizace hodnot (sekund)</div>
            <v-slider v-model="params.MQTTInterval_S" color="primary" :prepend-icon="`${mdiClockPlusOutline}`"
              :rules="[rules.required]" :max="120" :min="2" :step="1" thumb-label>
              <template v-slot:append>
                <v-text-field hide-details density="compact" style="width: 100px" v-model="params.MQTTInterval_S"
                  :rules="[rules.required]" variant="outlined" type="number" :max="120" :min="2"
                  :step="1"></v-text-field>
              </template>
            </v-slider>
          </div>
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
  BrokerPort: 1883,
  BrokerAdresa: "",
  MQTTbase: "",
  MQTTOn: false,
  MQTTInterval_S: 10,
  MQTTJmeno: "",
  MQTTHeslo: ""
};

export default {
  mixins: [baseParametersMixin],
  name: 'MQTTSettings',
  data() {
    return {
      params: dataFields,
    };
  },
}
</script>
