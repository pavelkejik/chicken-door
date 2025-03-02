<template>
  <v-card elevation="4">
    <div class="card-title-container ma-4">
      <div class="title-line"></div>
      <v-card-title class="headline text-center">Stav WiFi</v-card-title>
      <div class="title-line"></div>
    </div>
    <v-card-text>

      <v-skeleton-loader v-if="loading"
        type="chip, ossein, ossein, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, chip, ossein, ossein, avatar, list-item-two-line, divider"></v-skeleton-loader>

      <v-list v-else lines="two" item-props>

        <v-list-subheader>Připojení k síti</v-list-subheader>
        <v-list-item>
          <template v-slot:prepend>
            <v-avatar :color="wifiColor">
              <v-icon>{{ wifiStatusIcon }}</v-icon>
            </v-avatar>
          </template>

          <template v-slot:append>

            <v-switch v-model="params.WiFiSTAOn" hide-details inset color="primary"
              @change="() => submitDataList(['WiFiSTAOn'], false)">
              <template #prepend>
                <v-label :text="`${params.WiFiSTAOn ? 'ON' : 'OFF'}`"></v-label>
              </template>
            </v-switch>

          </template>

          <v-list-item-content>
            <v-list-item-title class="text-wrap">Status</v-list-item-title>
            <v-list-item-subtitle>{{ connectString }}</v-list-item-subtitle>
          </v-list-item-content>

        </v-list-item>

        <v-divider inset></v-divider>


        <v-list-item v-if="!hideWiFiDetails">
          <template v-slot:prepend>
            <v-avatar color="blue-grey-darken-2">
              <v-icon>{{ wifiStrengthIcon }}</v-icon>
            </v-avatar>
          </template>

          <v-list-item-content>
            <v-list-item-title class="text-wrap">SSID (RSSI)</v-list-item-title>
            <v-list-item-subtitle>{{ ssidString }}</v-list-item-subtitle>
          </v-list-item-content>

        </v-list-item>
        <v-divider v-if="!hideWiFiDetails" inset></v-divider>

        <v-list-item v-if="!hideWiFiDetails">
          <template v-slot:prepend>
            <v-avatar color="grey-darken-1">
              IP
            </v-avatar>
          </template>

          <v-list-item-content>
            <v-list-item-title class="text-wrap">IP Adresa</v-list-item-title>
            <v-list-item-subtitle>{{ params.LocalIP }}</v-list-item-subtitle>
          </v-list-item-content>

        </v-list-item>
        <v-divider inset></v-divider>

        <v-list-item v-if="!hideWiFiDetails">
          <template v-slot:prepend>
            <v-avatar color="grey-darken-1">
              <v-icon>{{ mdiHubOutline }}</v-icon>
            </v-avatar>
          </template>

          <v-list-item-content>
            <v-list-item-title class="text-wrap">MAC Adresa</v-list-item-title>
            <v-list-item-subtitle>{{ params.MacAdresa }}</v-list-item-subtitle>
          </v-list-item-content>

        </v-list-item>
        <v-divider v-if="!hideWiFiDetails" inset></v-divider>

        <v-list-item v-if="!hideWiFiDetails">
          <template v-slot:prepend>
            <v-avatar color="grey-darken-1">
              #
            </v-avatar>
          </template>

          <v-list-item-content>
            <v-list-item-title class="text-wrap">Maska podsítě</v-list-item-title>
            <v-list-item-subtitle>{{ params.Mask }}</v-list-item-subtitle>
          </v-list-item-content>

        </v-list-item>
        <v-divider v-if="!hideWiFiDetails" inset></v-divider>

        <v-list-item v-if="!hideWiFiDetails">
          <template v-slot:prepend>
            <v-avatar color="grey-darken-1">
              <v-icon>{{ mdiRouterNetwork }}</v-icon>
            </v-avatar>
          </template>

          <v-list-item-content>
            <v-list-item-title class="text-wrap">Výchozí brána</v-list-item-title>
            <v-list-item-subtitle>{{ params.Gateway }}</v-list-item-subtitle>
          </v-list-item-content>

        </v-list-item>
        <v-divider v-if="!hideWiFiDetails" inset></v-divider>

        <v-list-item v-if="!hideWiFiDetails">
          <template v-slot:prepend>
            <v-avatar color="grey-darken-1">
              <v-icon>{{ mdiServer }}</v-icon>
            </v-avatar>
          </template>

          <v-list-item-content>
            <v-list-item-title class="text-wrap">DNS servery</v-list-item-title>
            <v-list-item-subtitle>{{ dnsString }}</v-list-item-subtitle>
          </v-list-item-content>

        </v-list-item>
        <v-divider v-if="!hideWiFiDetails" inset></v-divider>


        <v-divider class="mt-9 mb-5"></v-divider>

        <v-list-subheader>Access point</v-list-subheader>
        <v-list-item>
          <template v-slot:prepend>
            <v-avatar :color="`${params.WiFiAPOn ? 'light-blue-darken-4' : 'grey-darken-1'}`">
              <v-icon>{{ wifiAPIcon }}</v-icon>
            </v-avatar>
          </template>

          <template v-slot:append>
            <v-switch v-model="params.WiFiAPOn" hide-details inset color="primary"
              @change="() => submitDataList(['WiFiAPOn'], false)">
              <template #prepend>
                <v-label :text="`${params.WiFiAPOn ? 'ON' : 'OFF'}`"></v-label>
              </template>
            </v-switch>
          </template>

          <v-list-item-content>
            <v-list-item-title class="text-wrap">Status</v-list-item-title>
            <v-list-item-subtitle>{{ wifiAPString }}</v-list-item-subtitle>
          </v-list-item-content>

        </v-list-item>

        <v-divider inset></v-divider>

        <v-list-item v-if="params.WiFiAPOn">
          <template v-slot:prepend>
            <v-avatar color="blue-grey-darken-2">
              <v-icon>{{ mdiDevices }}</v-icon>
            </v-avatar>
          </template>

          <v-list-item-content>
            <v-list-item-title class="text-wrap">Počet připojených zařízení</v-list-item-title>
            <v-list-item-subtitle>{{ params.WiFiAPClients }}</v-list-item-subtitle>
          </v-list-item-content>

        </v-list-item>
        <v-divider v-if="params.WiFiAPOn" inset></v-divider>

      </v-list>

    </v-card-text>

  </v-card>
</template>

<script>
import baseParametersMixin from '@/mixins/baseParametersMixin';

const dataFields = {
  WiFiSTAOn: false,
  WiFiAPOn: false,
  WiFihostname: "",
  WiFiSSID: "",
  LocalIP: "",
  Gateway: "",
  Mask: "",
  DNS1: "",
  DNS2: "",
  MacAdresa: "",
  WiFiRSSI: 0,
  WiFiStatus: 0,
  WiFiAPClients: 0,
};


export default {
  mixins: [baseParametersMixin],
  name: 'WiFiStatus',
  data: () => ({
    params: dataFields,
    fetchPeriod: 2000,
  }),
  computed: {
    hideWiFiDetails() {
      switch (this.params.WiFiStatus) {
        case 0: return true;
        case 1: return true;
        case 2: return false;
        case 3: return true;
        default: return this.params.WiFiSTAOn ? true : false;
      }
    },
    connectString() {
      switch (this.params.WiFiStatus) {
        case 0: return this.params.WiFiSTAOn ? 'Připojování': 'Vypnuto';
        case 1: return 'Připojování';
        case 2: return 'Připojeno';
        case 3: return 'Odpojeno';
        default: return this.params.WiFiSTAOn ? 'Hledání WiFi' : 'Vypnuto';
      }
    },
    ssidString() {
      return `${this.params.WiFiSSID} (${this.params.WiFiRSSI} dBm)`;
    },
    dnsString() {
      return `${this.params.DNS1}, ${this.params.DNS2}`;
    },

    wifiColor() {
      switch (this.params.WiFiStatus) {
        case 0: return this.params.WiFiSTAOn ? "light-blue-darken-3" : "grey-darken-1";
        case 1: return "light-blue-darken-3";
        case 2: return "green";
        case 3: return 'red';
        default: return this.params.WiFiSTAOn ? "green" : "grey-darken-1";
      }
    },
    wifiStatusIcon() {
      switch (this.params.WiFiStatus) {
        case 0: return this.params.WiFiSTAOn ? this.mdiWifiSettings : this.mdiWifiOff;
        case 1: return this.mdiWifiSettings;
        case 2: return this.mdiWifiCheck;
        case 3: return this.mdiWifiAlert;
        default: return this.params.WiFiSTAOn ? this.mdiWifiSettings : this.mdiWifiOff;
      }
    },
    wifiStrengthIcon() {
      if (this.params.WiFiRSSI > -50) {
        return this.mdiWifiStrength4;
      }
      else if (this.params.WiFiRSSI > -60) {
        return this.mdiWifiStrength3;

      }
      else if (this.params.WiFiRSSI > -70) {
        return this.mdiWifiStrength2;

      }
      else {
        return this.mdiWifiStrength1;

      }
    },
    wifiAPIcon() {
      return this.params.WiFiAPOn ? this.mdiAccessPointCheck : this.mdiAccessPointOff;
    },
    wifiAPString() {
      return this.params.WiFiAPOn ? `Zapnuto (název WiFi: ${this.params.WiFihostname})` : 'Vypnuto';
    }
  },
  // mounted() {
  //   this.fetchInterval = setInterval(() => {
  //     this.fetchData(false);
  //   }, 3000);
  // },
  // beforeUnmount() {
  //   clearInterval(this.fetchInterval);
  // },
};
</script>
