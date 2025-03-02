<template>
  <v-card elevation="4">
    <div class="card-title-container ma-4">
      <div class="title-line"></div>
      <v-card-title class="headline text-center">Stav MQTT</v-card-title>
      <div class="title-line"></div>
    </div>
    <v-card-text>

      <v-skeleton-loader v-if="loading"
        type="avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider, avatar, list-item-two-line, divider"></v-skeleton-loader>

      <v-list v-else lines="two" item-props>

        <v-list-subheader>Status</v-list-subheader>
        <v-list-item>
          <template v-slot:prepend>
            <v-avatar :color="MQTTColor">
              <v-icon>{{ mdiHubOutline }}</v-icon>
            </v-avatar>
          </template>

          <v-list-item-content>
            <v-list-item-title class="text-wrap">Status</v-list-item-title>
            <v-list-item-subtitle>{{ connectString }}</v-list-item-subtitle>
          </v-list-item-content>

        </v-list-item>

        <v-divider inset></v-divider>



        <v-list-item v-if="!hideMQTTDetails">
          <template v-slot:prepend>
            <v-avatar color="grey-darken-1">
              IP
            </v-avatar>
          </template>

          <v-list-item-content>
            <v-list-item-title class="text-wrap">IP adresa brokeru</v-list-item-title>
            <v-list-item-subtitle>{{ params.BrokerAdresa }}</v-list-item-subtitle>
          </v-list-item-content>

        </v-list-item>
        <v-divider inset v-if="!hideMQTTDetails"></v-divider>

        <v-list-item v-if="!hideMQTTDetails">
          <template v-slot:prepend>
            <v-avatar color="grey-darken-1">
              <v-icon>{{ mdiNetworkOutline }}</v-icon>
            </v-avatar>
          </template>

          <v-list-item-content>
            <v-list-item-title class="text-wrap">Port brokeru</v-list-item-title>
            <v-list-item-subtitle>{{ params.BrokerPort }}</v-list-item-subtitle>
          </v-list-item-content>

        </v-list-item>
        <v-divider v-if="!hideMQTTDetails" inset></v-divider>

        <v-list-item v-if="!hideMQTTDetails">
          <template v-slot:prepend>
            <v-avatar color="grey-darken-1">
              #
            </v-avatar>
          </template>

          <v-list-item-content>
            <v-list-item-title class="text-wrap">Klient ID/Base topic</v-list-item-title>
            <v-list-item-subtitle>{{ params.MQTTbase }}</v-list-item-subtitle>
          </v-list-item-content>

        </v-list-item>
        <v-divider v-if="!hideMQTTDetails" inset></v-divider>

      </v-list>

    </v-card-text>

  </v-card>
</template>

<script>
import baseParametersMixin from '@/mixins/baseParametersMixin';

const dataFields = {
  BrokerPort: "",
  BrokerAdresa: "",
  MQTTbase: "",
  MQTTStatus: 0,
};


export default {
  mixins: [baseParametersMixin],
  name: 'MQTTStatus',
  data: () => ({
    params: dataFields,
    fetchPeriod: 2000,
  }),
  computed: {
    hideMQTTDetails() {
      switch (this.params.MQTTStatus) {
        case 0: return true;
        case 1: return false;
        case 2: return false;
        case 3: return false;
        default: return false;
      }
    },
    connectString() {
      switch (this.params.MQTTStatus) {
        case 0: return 'Vypnuto';
        case 1: return 'Připojování';
        case 2: return 'Připojeno';
        case 3: return 'Odpojeno';
        default: return 'Vypnuto';
      }
    },

    MQTTColor() {
      switch (this.params.MQTTStatus) {
        case 0: return "grey-darken-1";
        case 1: return "light-blue-darken-3";
        case 2: return "green";
        case 3: return 'red';
        default: return "grey-darken-1";
      }
    },
  },
};
</script>
