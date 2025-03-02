<template>
  <v-tabs grow v-model="activeTab" align-tabs="center">
    <v-tab color="primary" :to="{ name: 'WiFi_Status' }">Stav</v-tab>
    <v-tab color="primary" :to="{ name: 'WiFi_Scan' }">Dostupné sítě</v-tab>
    <v-tab color="primary" :to="{ name: 'WiFi_Settings' }">Nastavení</v-tab>
  </v-tabs>
  <router-view></router-view>

</template>

<script>
export default {
  name: 'WifiView',
  computed: {
    activeTab: {
      get() {
        switch (this.$route.name) {
          case 'WiFi_Status': return 0;
          case 'WiFi_Scan': return 1;
          case 'WiFi_Settings': return 2;
          default: return 0;
        }
      },
      set(value) {
      }
    }
  }
};
</script>
