<template>
  <v-card elevation="4">
    <div class="card-title-container ma-4">
      <div class="title-line"></div>
      <v-card-title class="headline text-center">Nastavení připojení k WiFi</v-card-title>
      <div class="title-line"></div>
    </div>

    <v-form v-model="isFormValid" @submit.prevent="submitData">
      <v-skeleton-loader :loading="loading" type="chip, ossein, heading, heading, chip, ossein, heading, button">
        <v-card-text>
          <v-card-title class="form-element">WiFi</v-card-title>
          <v-text-field class="form-element" clearable v-model="params.WiFiSSID" variant="outlined"
            label="SSID (Název WiFi)"></v-text-field>
          <v-text-field class="form-element" v-model="params.WiFiHeslo" type="password" variant="outlined"
            label="Heslo"></v-text-field>

          <v-card-title class="form-element">Obecné nastavení</v-card-title>
          <v-text-field class="form-element" v-model="params.WiFihostname" variant="outlined" label="Hostname"
            :rules="[rules.required]"></v-text-field>
          <v-checkbox color="primary" v-model="params.StaticIP" label="Použít pevnou IP adresu"></v-checkbox>
          <div v-if="params.StaticIP">
            <v-text-field class="form-element" v-model="params.LocalIP" :rules="[rules.required, rules.ipAddress]"
              variant="outlined" label="Statická IP"></v-text-field>
            <v-text-field class="form-element" v-model="params.Gateway" :rules="[rules.required, rules.ipAddress]"
              variant="outlined" label="Výchozí brána"></v-text-field>
            <v-text-field class="form-element" v-model="params.Mask" :rules="[rules.required, rules.ipAddress]"
              variant="outlined" label="Maska podsítě"></v-text-field>
            <v-text-field class="form-element" v-model="params.DNS1" :rules="[rules.required, rules.ipAddress]"
              variant="outlined" label="DNS #1"></v-text-field>
            <v-text-field class="form-element" v-model="params.DNS2" :rules="[rules.required, rules.ipAddress]"
              variant="outlined" label="DNS #2"></v-text-field>
          </div>

          <v-checkbox color="primary" v-model="params.CaptivePortal"
            label="Povolit captive portal (experimentální)"></v-checkbox>
          <div v-if="params.CaptivePortal != originalParams.CaptivePortal"
            class="text-caption text-medium-emphasis ml-2 mb-4">
            <p class="text-yellow-darken-3">Pro uplatnění změn je potřeba zařízení restartovat.</p>
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
  WiFiSSID: '',
  WiFiHeslo: '',
  WiFihostname: '',
  StaticIP: false,
  LocalIP: '',
  Gateway: '',
  Mask: '',
  DNS1: '',
  DNS2: '',
  CaptivePortal: false,
};

export default {
  mixins: [baseParametersMixin],
  name: 'WiFiSettings',
  data() {
    return {
      params: dataFields,
    };
  },
}
</script>
  
