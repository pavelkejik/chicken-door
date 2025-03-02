<template>
  <!-- ... -->
    <v-tabs grow v-model="activeTab" align-tabs="center">
      <v-tab color="primary" :to="{ name: 'Settings_Auto' }">Automatika</v-tab>
      <v-tab :disabled="accessLevel != 1"  color="primary" :to="{ name: 'Settings_Motor' }">Pohon</v-tab>
    </v-tabs>
      <router-view></router-view>
  <!-- ... -->
</template>

<script>
export default {
  name: 'SettingsView',
  computed: {
    activeTab: {
      get() {
        switch (this.$route.name) {
          case 'Settings_Auto': return 0;
          case 'Settings_Motor': return 1;
          default: return 0;
        }
      },
      set(value) {
      }
    }
  }
};
</script>
