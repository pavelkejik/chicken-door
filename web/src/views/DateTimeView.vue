<template>
    <!-- ... -->
      <v-tabs grow v-model="activeTab" align-tabs="center">
        <v-tab color="primary" :to="{ name: 'DateTime_Status' }">Stav</v-tab>
        <v-tab :disabled="accessLevel != 1" color="primary" :to="{ name: 'DateTime_Settings' }">Nastavení</v-tab>
      </v-tabs>
  
        <router-view></router-view>
    <!-- ... -->
  </template>
  
  <script>
  export default {
    name: 'DateTimeView',
    computed: {
      activeTab: {
        get() {
          switch (this.$route.name) {
            case 'DateTime_Status': return 0;
            case 'DateTime_Settings': return 1;
            default: return 0;
          }
        },
        set(value) {
        }
      }
    }
  };
  </script>
  