<template>
  <!-- ... -->
  <v-tabs grow v-model="activeTab" align-tabs="center">
    <v-tab color="primary" :to="{ name: 'MQTT_Status' }">Stav</v-tab>
    <v-tab color="primary" :to="{ name: 'MQTT_Settings' }">Nastavení</v-tab>
  </v-tabs>
  <router-view></router-view>

  <!-- ... -->
</template>

<script>
export default {
  name: 'MQTTView',
  computed: {
    activeTab: {
      get() {
        switch (this.$route.name) {
          case 'MQTT_Status': return 0;
          case 'MQTT_Settings': return 2;
          default: return 0;
        }
      },
      set(value) {
      }
    }
  }
};
</script>
