<template>
  <v-app color="blue">

    <the-sidebar :drawer="drawer" @update:drawer="drawer = $event" :rail="rail"></the-sidebar>
    <the-topbar @toggle-drawer="toggleDrawer"></the-topbar>
    <!-- Main content -->
    <v-main  id="content">
      <v-container id="page-container" fluid class="container px-3 px-sm-6 py-sm-6 mx-auto">
      <router-view></router-view>
      </v-container>
    </v-main>

  </v-app>
</template>

<script>
import TheTopbar from './components/TheTopbar.vue';
import TheSidebar from './components/TheSidebar.vue';
import ResizeMixin from './mixins/ResizeHandler';

export default {
  name: 'App',
  components: {
    TheTopbar,
    TheSidebar
  },
  mixins: [ResizeMixin],
  data() {
    return {
      drawer: false, // Controls visibility of the sidebar on mobile
      rail: true,    // Controls whether the sidebar is in mini-variant (icon only) mode
      accessLevel: 'guest', // Default access level
    };
  },
  methods: {
    toggleDrawer() {
      // For mobile, toggle the drawer visibility
      if (this.isMobile) {
        this.rail = false;
        this.drawer = !this.drawer;
      } else {
        // For desktop, toggle between rail (icons only) and expanded sidebar
        this.rail = !this.rail;
        this.drawer = true;
      }
    },
  }
};
</script>
