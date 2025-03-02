/**
 * main.js
 *
 * Bootstraps Vuetify and other plugins then mounts the App`
 */

// Plugins
import { registerPlugins } from '@/plugins'
// import './assets/fonts/roboto.css'; // Adjust the path to match where you placed the file

// Components
import App from './App.vue'

// Composables
import { createApp } from 'vue'
import router from './router';

import { ToastPlugin } from './plugins/toast';
import "vue-toastification/dist/index.css";
import "./assets/styles/toast.scss";
import '@/assets/styles/global.scss';
import accessLevelMixin from '@/mixins/accessLevelMixin';
import axios from 'axios';
import { updateAccessLevel } from '@/utils/store';

function fetchAccessLevel() {
    return axios.get('/api/get_params?AccessLvl=')
        .then(response => {
            if (response.data && 'AccessLvl' in response.data) {
                updateAccessLevel(response.data.AccessLvl);
                return response.data.AccessLvl;
              } else {
                throw new Error('AccessLvl key not found in response');
              }
        })
        .catch(error => {
            console.error('Failed to fetch access level:', error);
            updateAccessLevel(0);
            return 0; // Default to guest if fetch fails
        });
}
let accessFetchInterval = null;

fetchAccessLevel().then(() => {
    const app = createApp(App)
    app.mixin(accessLevelMixin);
    app.use(router).use(ToastPlugin);

    registerPlugins(app)

    accessFetchInterval = setInterval(fetchAccessLevel, 10000);
    app.mount('#app')
});
