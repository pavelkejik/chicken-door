import { createRouter, createWebHistory } from 'vue-router';
import HomeView from '../views/HomeView.vue';

import SettingsView from '../views/SettingsView.vue';
import SettingsAuto from '../views/SettingsAuto.vue';
import SettingsMotor from '../views/SettingsMotor.vue';

import WifiView from '../views/WifiView.vue';
import WiFiStatus from '../views/WiFiStatus.vue';
import WiFiScan from '../views/WiFiScan.vue';
import WiFiSettings from '../views/WiFiSettings.vue';

import DateTimeView from '../views/DateTimeView.vue';
import DateTimeStatus from '../views/DateTimeStatus.vue';
import DateTimeSettings from '../views/DateTimeSettings.vue';

import SystemView from '../views/SystemView.vue';
import SystemLog from '../views/SystemLog.vue';
import SystemStatus from '../views/SystemStatus.vue';
import SystemUpdate from '../views/SystemUpdate.vue';

import DevicesView from '../views/DevicesView.vue';

import MQTTView from '../views/MQTTView.vue';
import MQTTStatus from '../views/MQTTStatus.vue';
import MQTTSettings from '../views/MQTTSettings.vue';

const routes = [
  { path: '/', component: HomeView, meta: { title: 'Dvířka' } },

  { path: '/devices', component: DevicesView, meta: { title: 'Příslušenství' } },

  {
    path: '/settings', component: SettingsView,
    redirect: '/settings/auto',

    children: [
      {
        path: 'auto',
        component: SettingsAuto,
        name: 'Settings_Auto',
        meta: { title: 'Nastavení / Automatika' }
      },
      {
        path: 'motor',
        component: SettingsMotor,
        name: 'Settings_Motor',
        meta: { title: 'Nastavení / Pohon',  requiredAccessLevel: 1 }
      }
    ]
  },

  {
    path: '/wifi', component: WifiView,
    redirect: '/wifi/status',

    children: [
      {
        path: 'status',
        component: WiFiStatus,
        name: 'WiFi_Status',
        meta: { title: 'WiFi / Stav' }
      },
      {
        path: 'scan',
        component: WiFiScan,
        name: 'WiFi_Scan',
        meta: { title: 'WiFi / Sken' }
      },
      {
        path: 'settings',
        component: WiFiSettings,
        name: 'WiFi_Settings',
        meta: { title: 'WiFi / Nastavení' }
      }
    ]
  },
  {
    path: '/mqtt', component: MQTTView,
    redirect: '/mqtt/status',

    children: [
      {
        path: 'status',
        component: MQTTStatus,
        name: 'MQTT_Status',
        meta: { title: 'MQTT / Stav' }
      },
      {
        path: 'settings',
        component: MQTTSettings,
        name: 'MQTT_Settings',
        meta: { title: 'MQTT / Nastavení' }
      }
    ]
  },


  { path: '/date_time', component: DateTimeView, 
  redirect: '/date_time/status',

  children: [
    {
      path: 'status',
      component: DateTimeStatus,
      name: 'DateTime_Status',
      meta: { title: 'Datum a čas / Stav' }
    },
    {
      path: 'settings',
      component: DateTimeSettings,
      name: 'DateTime_Settings',
      meta: { title: 'Datum a čas / Nastavení', requiredAccessLevel: 1 }
    }
  ]
},


  { path: '/system', component: SystemView, 
  redirect: '/system/status',

  children: [
    {
      path: 'status',
      component: SystemStatus,
      name: 'System_Status',
      meta: { title: 'Systém / Stav',  requiredAccessLevel: 1 }
    },
    {
      path: 'log',
      component: SystemLog,
      name: 'System_Log',
      meta: { title: 'Systém / Log',  requiredAccessLevel: 1}
    },
    {
      path: 'update',
      component: SystemUpdate,
      name: 'System_Update',
      meta: { title: 'Systém / Aktualizace',  requiredAccessLevel: 1 }
    }
  ]
}


];

const router = createRouter({
  history: createWebHistory(),
  routes,
});

router.beforeEach((to, from, next) => {
  const requiredAccessLevel = to.matched.some(record => record.meta.requiredAccessLevel) ? to.matched.find(record => record.meta.requiredAccessLevel).meta.requiredAccessLevel : null;
  
  const userAccessLevel = localStorage.getItem('accessLevel');

  if ( requiredAccessLevel && (!userAccessLevel || userAccessLevel != requiredAccessLevel)) {
    next('/');
  } else {
    next();
  }
});

export default router;
