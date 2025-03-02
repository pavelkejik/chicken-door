<template>
  <v-app-bar elevation="1" app>
    <v-app-bar-nav-icon @click="toggleDrawer"></v-app-bar-nav-icon>
    <v-toolbar-title>{{ pageTitle }}</v-toolbar-title>
    <v-spacer></v-spacer>
    <v-icon class="mr-2" @click="toggleTheme">
      {{ mdiThemeLightDark }}
    </v-icon>


    <v-menu v-model="menu" :close-on-content-click="false" location="end">
      <template v-slot:activator="{ props }">
        <v-icon class="ml-2 mr-2" v-bind="props">
          {{ accessLevel == 1 ? mdiAccountWrench : mdiAccount }} 
        </v-icon>

      </template>

      <v-card min-width="320">
        <v-list>
          <v-list-item :title="`${accessLevel == 1 ? 'Servis' : 'Uživatel'}`">
            <template v-slot:prepend>
              <v-avatar color="grey-lighten-1">
                <v-icon>{{ accessLevel == 1 ? mdiAccountWrench : mdiAccount }}</v-icon>
              </v-avatar>
            </template>
          </v-list-item>
        </v-list>

        <v-divider></v-divider>

        <v-card-actions class="mt-1 mb-1">
          <v-spacer></v-spacer>
          <div v-if="accessLevel == 1">
            <v-btn color="warning" variant="outlined" @click="newPinDialog = true">
              Změnit PIN
            </v-btn>
            <v-btn color="red" variant="outlined" @click="logout">
              Ukončit servis
            </v-btn>
          </div>
          <v-btn v-else color="primary" variant="outlined" @click="adminDialog = true">
            Servis
          </v-btn>

          <v-spacer></v-spacer>

        </v-card-actions>
      </v-card>
    </v-menu>
  </v-app-bar>

  <v-dialog v-model="adminDialog">
    <v-sheet class="py-8 px-6 mx-auto ma-4 text-center" elevation="4" rounded="lg" max-width="500" width="100%">
      <h3 class="text-h5">PIN kód</h3>

      <div class="text-subtitle-2 font-weight-light mb-3">Prosím zadejte servisní PIN kód</div>

      <v-otp-input :error="validateError" :disabled="validating" v-model="pinCode" class="mb-8" divider="•" length="4"
        variant="outlined"></v-otp-input>

      <div class="text-caption">
        <v-btn :disabled="pinCode.length < 4" :loading="validating" rounded variant="outlined" text="Ověřit"
          color="primary" height="40" width="135" @click="validatePin"></v-btn>
      </div>
    </v-sheet>
  </v-dialog>

  <v-dialog v-model="newPinDialog">
    <v-sheet class="py-8 px-6 mx-auto ma-4 text-center" elevation="4" rounded="lg" max-width="500" width="100%">
      <h3 class="text-h5">Nový PIN kód</h3>

      <div class="text-subtitle-2 font-weight-light mb-3">Prosím zadejte nový servisní PIN kód</div>

      <v-otp-input :disabled="newPinValidating" v-model="newPinCode" class="mb-8" divider="•" length="4"
        variant="outlined"></v-otp-input>

      <div class="text-caption">
        <v-btn :disabled="newPinCode.length < 4" :loading="newPinValidating" rounded variant="outlined" text="Uložit"
          color="primary" height="40" width="135" @click="saveNewPin"></v-btn>
      </div>
    </v-sheet>
  </v-dialog>

</template>

<script>
import { updateAccessLevel } from '@/utils/store';
import axios from 'axios';

import {
  mdiThemeLightDark,
  mdiAccount,
  mdiAccountWrench,
} from '@mdi/js'



export default {
  name: 'TheTopbar',
  data: () => ({
    mdiThemeLightDark,
    mdiAccount,
    mdiAccountWrench,
    adminDialog: false,
    newPinDialog: false,
    pinCode: '',
    newPinCode: '',
    validateError: false,
    newPinValidating: false,
    validating: false,
    fav: true,
    menu: false,
    message: false,
    hints: true,
    pageTitle: 'Dvířka',
  }),
  created() {
    this.updateTitle();
    this.$watch(
      () => this.$route,
      () => {
        this.updateTitle();
      },

      { immediate: true }
    )
  },
  methods: {
    toggleDrawer() {
      this.$emit('toggle-drawer');
    },
    toggleTheme() {
      const newTheme = this.$vuetify.theme.global.name == 'dark' ? 'light' : 'dark';

      // theme.global.name.value = newTheme;
      this.$vuetify.theme.global.name = newTheme;
      localStorage.setItem('themePreference', newTheme);
    },
    updateTitle() {
      this.pageTitle = this.$route.meta.title || 'Dvířka';
      if (this.$route.matched.some(record => record.meta.title)) {
        this.pageTitle = this.$route.matched.slice().reverse().find(record => record.meta.title).meta.title;
      }
    },

    validatePin() {
      this.validating = true; 

      axios.post('/api/set_params', { VstupPin: this.pinCode })
        .then(response => {
          updateAccessLevel(1);
          this.validateError = false;
          this.adminDialog = false;
        })
        .catch(error => {
          console.error('PIN validation failed');
          this.$toast.error('Nesprávný PIN kód.');
          this.validateError = true;
        })
        .finally(() => {
          this.validating = false; 
        });
    },

    logout() {
      axios.post('/api/set_params', { VstupPin: "-1" })
        .then(response => {
          updateAccessLevel(0);
          this.$toast.success('Servis byl ukončen.');

        })
        .catch(error => {
          this.$toast.error('Došlo k chybě při odesílání požadavku. Zkuste to znovu později.');
        })
        .finally(() => {
        });
    },

    saveNewPin() {
      this.newPinValidating = true;

      axios.post('/api/set_params', { PinServis: this.newPinCode })
        .then(response => {
          this.newPinDialog = false;
          this.$toast.success('Nový PIN kód byl úspěšně uložen.');
        })
        .catch(error => {
          this.$toast.error('Došlo k chybě při odesílání požadavku. Zkuste to znovu později.');
        })
        .finally(() => {
          this.newPinValidating = true;
        });
    },

    
  }
};
</script>
