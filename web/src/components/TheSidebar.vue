<template>
    <v-navigation-drawer color="customSidebarColor"  v-model="internalDrawer" :rail="rail" :mobile-breakpoint="'sm'" elevation="10" app >
      <v-list class="custom-sidebar" density="compact" nav>
        <v-list-item to="/" :prepend-icon="`${mdiViewDashboard}`" color="primary"  title="Dvířka"></v-list-item>
        <v-list-item to="/devices" :prepend-icon="`${mdiHomeAutomation}`" color="primary" title="Příslušenství"></v-list-item>
        <v-list-item to="/settings" :prepend-icon="`${mdiTune}`" color="primary" title="Nastavení"></v-list-item>
        <v-list-item to="/wifi" :prepend-icon="`${mdiWifiCog}`" color="primary" title="WiFi"></v-list-item>
        <v-list-item to="/mqtt" :prepend-icon="`${mdiHomeAssistant}`" color="primary" title="MQTT"></v-list-item>
        <v-list-item to="/date_time" :prepend-icon="`${mdiClockTimeFourOutline }`" color="primary" title="Datum a čas"></v-list-item>
        <v-list-item v-if="accessLevel==1" to="/system" :prepend-icon="`${mdiCog}`" color="primary" title="Systém"></v-list-item>
      </v-list>
    </v-navigation-drawer>
</template>

<script>
import {
  mdiViewDashboard ,
  mdiWifiCog ,
  mdiCog ,
  mdiTune ,
  mdiAntenna ,
  mdiClockTimeFourOutline ,
  mdiHomeAssistant,
  mdiHomeAutomation,
} from '@mdi/js'



export default {
  name: 'TheSidebar',
  props: {
    drawer: Boolean,
    rail: Boolean
  },
  data: () => ({
    mdiViewDashboard ,
    mdiWifiCog ,
    mdiCog ,
    mdiTune ,
    mdiAntenna ,
    mdiClockTimeFourOutline ,
    mdiHomeAssistant,
    mdiHomeAutomation,
  }),
  computed: {
    internalDrawer: {
      get() {
        return this.drawer;
      },
      set(value) {
        this.$emit('update:drawer', value);
      }
    }
  }
};
</script>