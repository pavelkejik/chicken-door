<template>
    <v-tabs grow v-model="activeTab" align-tabs="center">
      <v-tab color="primary" :to="{ name: 'System_Status' }">Stav</v-tab>
      <v-tab color="primary" :to="{ name: 'System_Log' }">Log</v-tab>
      <v-tab color="primary" :to="{ name: 'System_Update' }">Aktualizace firmwaru</v-tab>
    </v-tabs>

  
      <router-view></router-view>
</template>

<script>
export default {
  name: 'SystemView',
  computed: {
    activeTab: {
      get() {
        switch (this.$route.name) {
          case 'System_Status': return 0;
          case 'System_Log': return 1;
          case 'System_Update': return 2;
          default: return 0;
        }
      },
      set(value) {
      }
    }
  }
};
</script>
