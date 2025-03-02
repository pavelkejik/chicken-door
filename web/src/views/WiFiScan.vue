<template>
  <v-card elevation="4">
    <div class="card-title-container ma-4">
      <div class="title-line"></div>
      <v-card-title class="headline text-center">Dostupné sítě WiFi</v-card-title>
      <div class="title-line"></div>
    </div>

    <v-card-item>
      <v-skeleton-loader v-if="loading || params.WiFiScanCmd"
        type="avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, button"></v-skeleton-loader>

      <div v-else>
        <v-list lines="two" item-props>

          <v-list-item v-if="wifiShow(params.WiFiScanSSID1, params.WiFiScanRSSI1)">
            <template v-slot:prepend>
              <v-avatar color="blue-grey-darken-2">
                <v-icon>{{ wifiStrengthIcon(params.WiFiScanRSSI1) }}</v-icon>
              </v-avatar>
            </template>

            <v-list-item-content>
              <v-list-item-title class="text-wrap">{{ params.WiFiScanSSID1 }}</v-list-item-title>
              <v-list-item-subtitle>{{ wifiRSSIString(params.WiFiScanRSSI1) }}</v-list-item-subtitle>
            </v-list-item-content>

          </v-list-item>
          <v-divider v-if="wifiShow(params.WiFiScanSSID1, params.WiFiScanRSSI1)" inset></v-divider>

          <v-list-item v-if="wifiShow(params.WiFiScanSSID2, params.WiFiScanRSSI2)">
            <template v-slot:prepend>
              <v-avatar color="blue-grey-darken-2">
                <v-icon>{{ wifiStrengthIcon(params.WiFiScanRSSI2) }}</v-icon>
              </v-avatar>
            </template>

            <v-list-item-content>
              <v-list-item-title class="text-wrap">{{ params.WiFiScanSSID2 }}</v-list-item-title>
              <v-list-item-subtitle>{{ wifiRSSIString(params.WiFiScanRSSI2) }}</v-list-item-subtitle>
            </v-list-item-content>

          </v-list-item>
          <v-divider v-if="wifiShow(params.WiFiScanSSID2, params.WiFiScanRSSI2)" inset></v-divider>


          <v-list-item v-if="wifiShow(params.WiFiScanSSID3, params.WiFiScanRSSI3)">
            <template v-slot:prepend>
              <v-avatar color="blue-grey-darken-2">
                <v-icon>{{ wifiStrengthIcon(params.WiFiScanRSSI3) }}</v-icon>
              </v-avatar>
            </template>

            <v-list-item-content>
              <v-list-item-title class="text-wrap">{{ params.WiFiScanSSID3 }}</v-list-item-title>
              <v-list-item-subtitle>{{ wifiRSSIString(params.WiFiScanRSSI3) }}</v-list-item-subtitle>
            </v-list-item-content>

          </v-list-item>
          <v-divider v-if="wifiShow(params.WiFiScanSSID3, params.WiFiScanRSSI3)" inset></v-divider>

          <v-list-item v-if="wifiShow(params.WiFiScanSSID4, params.WiFiScanRSSI4)">
            <template v-slot:prepend>
              <v-avatar color="blue-grey-darken-2">
                <v-icon>{{ wifiStrengthIcon(params.WiFiScanRSSI4) }}</v-icon>
              </v-avatar>
            </template>

            <v-list-item-content>
              <v-list-item-title class="text-wrap">{{ params.WiFiScanSSID4 }}</v-list-item-title>
              <v-list-item-subtitle>{{ wifiRSSIString(params.WiFiScanRSSI4) }}</v-list-item-subtitle>
            </v-list-item-content>

          </v-list-item>
          <v-divider v-if="wifiShow(params.WiFiScanSSID4, params.WiFiScanRSSI4)" inset></v-divider>


          <v-list-item v-if="wifiShow(params.WiFiScanSSID5, params.WiFiScanRSSI5)">
            <template v-slot:prepend>
              <v-avatar color="blue-grey-darken-2">
                <v-icon>{{ wifiStrengthIcon(params.WiFiScanRSSI5) }}</v-icon>
              </v-avatar>
            </template>

            <v-list-item-content>
              <v-list-item-title class="text-wrap">{{ params.WiFiScanSSID5 }}</v-list-item-title>
              <v-list-item-subtitle>{{ wifiRSSIString(params.WiFiScanRSSI5) }}</v-list-item-subtitle>
            </v-list-item-content>

          </v-list-item>
          <v-divider v-if="wifiShow(params.WiFiScanSSID5, params.WiFiScanRSSI5)" inset></v-divider>

          <v-list-item v-if="wifiShow(params.WiFiScanSSID6, params.WiFiScanRSSI6)">
            <template v-slot:prepend>
              <v-avatar color="blue-grey-darken-2">
                <v-icon>{{ wifiStrengthIcon(params.WiFiScanRSSI6) }}</v-icon>
              </v-avatar>
            </template>

            <v-list-item-content>
              <v-list-item-title class="text-wrap">{{ params.WiFiScanSSID6 }}</v-list-item-title>
              <v-list-item-subtitle>{{ wifiRSSIString(params.WiFiScanRSSI6) }}</v-list-item-subtitle>
            </v-list-item-content>

          </v-list-item>
          <v-divider v-if="wifiShow(params.WiFiScanSSID6, params.WiFiScanRSSI6)" inset></v-divider>

          <v-list-item v-if="wifiShow(params.WiFiScanSSID7, params.WiFiScanRSSI7)">
            <template v-slot:prepend>
              <v-avatar color="blue-grey-darken-2">
                <v-icon>{{ wifiStrengthIcon(params.WiFiScanRSSI7) }}</v-icon>
              </v-avatar>
            </template>

            <v-list-item-content>
              <v-list-item-title class="text-wrap">{{ params.WiFiScanSSID7 }}</v-list-item-title>
              <v-list-item-subtitle>{{ wifiRSSIString(params.WiFiScanRSSI7) }}</v-list-item-subtitle>
            </v-list-item-content>

          </v-list-item>
          <v-divider v-if="wifiShow(params.WiFiScanSSID7, params.WiFiScanRSSI7)" inset></v-divider>

        </v-list>

        <v-card-actions>
          <v-btn v-if="!params.WiFiScanCmd && !loading" variant="outlined" color="primary"
            @click="() => { params.WiFiScanCmd = true; submitDataList(['WiFiScanCmd'], false); }">
            <v-icon left class="mr-2">{{ mdiWifiRefresh }}</v-icon>
            Znovu skenovat</v-btn>
        </v-card-actions>
      </div>
    </v-card-item>


  </v-card>
</template>

<script>
import baseParametersMixin from '@/mixins/baseParametersMixin';

const dataFields = {
  WiFiScanCmd: false,
  WiFiScanSSID1: "",
  WiFiScanRSSI1: 0,
  WiFiScanSSID2: "",
  WiFiScanRSSI2: 0,
  WiFiScanSSID3: "",
  WiFiScanRSSI3: 0,
  WiFiScanSSID4: "",
  WiFiScanRSSI4: 0,
  WiFiScanSSID5: "",
  WiFiScanRSSI5: 0,
  WiFiScanSSID6: "",
  WiFiScanRSSI6: 0,
  WiFiScanSSID7: "",
  WiFiScanRSSI7: 0,
};


export default {
  mixins: [baseParametersMixin],
  name: 'WiFiScan',
  data: () => ({
    params: dataFields,
  }),
  methods:
  {
    wifiStrengthIcon(rssi) {
      if (rssi > -50) {
        return this.mdiWifiStrength4;
      }
      else if (rssi > -60) {
        return this.mdiWifiStrength3;

      }
      else if (rssi > -70) {
        return this.mdiWifiStrength2;

      }
      else {
        return this.mdiWifiStrength1;

      }
    },
    wifiShow(ssid, rssi) {
      return (rssi !== 0) && (ssid != '');
    },
    wifiRSSIString(rssi) {
      return `RSSI: ${rssi} dBm`;
    }
  },
  mounted() {
    this.fetchInterval = setInterval(() => {
      if (this.params.WiFiScanCmd) {
        this.fetchData(false);
      }
    }, 3000);
  },
  beforeUnmount() {
    clearInterval(this.fetchInterval);
  },
};
</script>
