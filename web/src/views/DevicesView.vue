<template>
  <!-- Devices List -->
  <v-row>

    <v-col v-if="loading" v-for="n in 4" :key="`skeleton-${n}`" cols="12" sm="6" md="4" lg="3" xxl="2">
      <v-skeleton-loader class="mx-auto" type="table-heading, image, list-item" height="320"></v-skeleton-loader>
    </v-col>

    <v-col v-else v-for="(device, index) in devices" :key="device.mac" cols="12" sm="6" md="4" lg="3" xxl="2">

      <v-card :elevation="device.pair == '0' ? 12 : 2" class="pa-2" @click="selectDevice(index)"
        :class="{ 'new-device': device.pair == '0' }">

        <v-card-title class="d-flex justify-space-between align-center">
          <span class="font-weight-bold">{{ device.name }}</span>
          <v-chip :color="device.pair == '0' || device.pair == '1' ? 'primary' : (device.on ? 'green' : 'red')"
            :class="{ 'blink-slow': device.pair == '1' }" variant="tonal">
            {{ device.pair == '0' ? 'Nové!' : (device.pair == '1' ? 'Párování' : (device.on ? 'Připojeno' :
              'Neodpovídá')) }}
          </v-chip>
        </v-card-title>



        <!-- Camera handling: device.type == 2 -->
        <template v-if="device.type == 2">


          <div v-if="device.pair == '2'" class="image-container">
            <v-img v-if="device.pair == '2'"
              :src="`api/get_device_image?id=${index}&timestamp=${cameraTimestamps[index] || Date.now()}`"
              height="220"></v-img>
            <!-- Timestamp overlay for each camera -->
            <!-- <div v-if="cameraTimestamps[index]" class="timestamp-overlay">
              {{ new Date(cameraTimestamps[index]).toLocaleString() }}
            </div> -->
          </div>
          <v-container v-else class="d-flex justify-center align-center" style="height: 220px;">
            <v-icon size="120" large color="grey lighten-1">{{ mdiCamera }}</v-icon>
          </v-container>



        </template>
        <template v-else-if="device.type == 3">
          <div v-if="device.pair == '2'" class="image-container">
            <v-img v-if="device.pair == '2'"
              :src="`api/get_device_image?id=${index}&timestamp=${cameraTimestamps[index] || Date.now()}`"
              height="220"></v-img>
            <!-- Timestamp overlay for each camera -->
            <!-- <div v-if="cameraTimestamps[index]" class="timestamp-overlay">
              {{ new Date(cameraTimestamps[index]).toLocaleString() }}
            </div> -->
          </div>
          <v-container v-else class="d-flex justify-center align-center" style="height: 220px;">
            <v-icon size="120" large color="grey lighten-1">{{ mdiCamera }}</v-icon>
          </v-container>
        </template>

        <!-- For devices with type != 2, show static image -->
        <v-img v-else src="../assets/feeder.PNG" height="220"></v-img>

        <!-- <v-card-subtitle>MAC:{{ device.mac }}</v-card-subtitle> -->
      </v-card>

    </v-col>
  </v-row>

  <v-dialog v-model="pairDialog" persistent max-width="500px">
    <v-card>
      <v-card-title>
        Spárovat zařízení
      </v-card-title>
      <v-card-text>
        <div class="text-caption text-medium-emphasis ml-2 mb-4">
          <v-row>
            <v-col cols="4" class="text-start pr-2">MAC:</v-col>
            <v-col cols="8" class="text-start">{{ selectedDevice.mac }}</v-col>
          </v-row>
          <v-row class="my-1">
            <v-col cols="4" class="text-start pr-2">Typ zařízení:</v-col>
            <v-col cols="8" class="text-start">{{ formatDeviceType(selectedDevice.type) }}</v-col>
          </v-row>
        </div>



        <v-text-field label="Název zařízení" v-model="tempDeviceName"></v-text-field>

      </v-card-text>
      <v-card-actions>
        <v-spacer></v-spacer>
        <v-btn class="mr-2" variant="outlined" color="primary" @click="closePairDialog">
          <v-icon left class="mr-2">{{ mdiCloseCircle }}</v-icon>
          Zavřít
        </v-btn>
        <v-btn variant="tonal" color="cyan-lighten-4" :loading="selectedDevice.pair == '1'" @click="pairDevice">
          <v-icon left color="warning" class="mr-2">{{ mdiConnection }}</v-icon>
          Spárovat
        </v-btn>
      </v-card-actions>
    </v-card>
  </v-dialog>
  <v-navigation-drawer touchless v-model="devDrawer" temporary location="right" width="600"
    class="device-details-drawer mt-2 ">
    <template v-slot:prepend>
      <div v-if="selectedDevice" class="d-flex justify-space-between align-center">

        <v-btn class="ma-2" density="compact" icon @click="devDrawer = false">
          <v-icon>{{ mdiCloseCircle }}</v-icon>
        </v-btn>
        <div class="mr-3 mt-2 mb-2">
          <div v-if="(selectedDevice.sleep != -1) && (selectedDevice.on)">
            <v-icon color="primary" left class="mr-2">{{ mdiSleep }}</v-icon>

            <v-progress-circular :model-value="sleepProgress" :rotate="360" :size="40" :width="5" color="primary">
              {{ sleepTime }}
            </v-progress-circular>
          </div>
          <div v-else>
            <v-icon size="32" :color="selectedDevice.on ? 'green' : 'red'" left class="mr-2 mb-2">{{ selectedDevice.on ?
              mdiAccessPointCheck : mdiAccessPointOff
            }}</v-icon>
          </div>
        </div>
      </div>
    </template>


    <v-card>
      <v-tabs grow align-tabs="center" v-model="deviceTab" @update:model-value="tabChanged">
        <v-tab color="primary" value="overview">Přehled</v-tab>
        <v-tab color="primary" value="parameters">Parametry</v-tab>
        <v-tab color="primary" value="info">O zařízení</v-tab>
        <v-tab v-if="accessLevel == 1" color="primary" value="log">Log</v-tab>
        <v-tab v-if="accessLevel == 1" color="primary" value="update">Aktualizace</v-tab>
      </v-tabs>

      <v-card-text v-if="devDrawer">
        <v-window v-model="deviceTab">

          <v-window-item value="overview">
            <component :is="currentDeviceComponent" ref="currentDeviceView" :device-id="selectedDeviceIndex">
            </component>
          </v-window-item>

          <v-window-item value="parameters">
            <!-- Parameters DataTable -->
            <v-data-table-virtual :loading="device_loading" :headers="parameterHeaders" :items="parameters"
              class="elevation-1" :hover=true @click:row="handleRowClick">

              <template v-slot:item.val="{ item }">
                {{ getEnumData(item).text }}
              </template>


              <template v-slot:item.writable="{ item }">
                <v-icon v-if="item.writable && !isInstallerWritable(item.dsc)" color="primary">
                  {{ mdiPencil }}
                </v-icon>
                <v-icon v-else-if="item.writable && isInstallerWritable(item.dsc) && accessLevel == 1" color="primary">
                  {{ mdiPencil }}
                </v-icon>
                <v-icon v-else-if="item.writable && isInstallerWritable(item.dsc) && accessLevel != 1" color="grey">
                  {{ mdiPencilLock }}
                </v-icon>
              </template>


            </v-data-table-virtual>

            <!-- Editing Dialog -->
            <v-dialog v-model="editDialog" persistent max-width="500px">
              <v-form v-model="isFormValid" @submit.prevent="saveEdit">
                <v-card>
                  <v-card-title>
                    Změnit hodnotu
                  </v-card-title>
                  <v-divider></v-divider>
                  <v-card-text>
                    <v-container>
                      <v-row>
                        <v-col cols="12">
                          <template v-if="isEnumParameter(editedItem)">
                            <v-select :items="getEnumData(editedItem).options" v-model="editedItem.val"
                              item-value="value" item-title="text" :label="`${editedItem.str}`"
                              :disabled="!editedItem.writable || (isInstallerWritable(editedItem.dsc) && accessLevel != 1)">
                            </v-select>
                          </template>
                          <template v-else>
                            <!-- <v-text-field v-model="editedItem.val" :label="`${editedItem.str}`" :type="getInputType(editedItem.type)" :disabled="!editedItem.writable"></v-text-field> -->
                            <v-text-field v-if="editedItem.atr & ParAtr.CHART_FLAG" v-model="chartValue"
                              variant="outlined" :type="getInputType(editedItem.type)" :label="`${editedItem.str}`"
                              :disabled="!editedItem.writable || (isInstallerWritable(editedItem.dsc) && accessLevel != 1)"></v-text-field>
                            <v-text-field v-else v-model="editedItem.val" variant="outlined"
                              :type="getInputType(editedItem.type)" :min="editedItem.min" :max="editedItem.max" step="1"
                              :label="`${editedItem.str}`" :rules="valueRules()"
                              :disabled="!editedItem.writable || (isInstallerWritable(editedItem.dsc) && accessLevel != 1)"></v-text-field>
                          </template>
                        </v-col>
                      </v-row>
                    </v-container>
                  </v-card-text>
                  <v-divider></v-divider>
                  <v-card-actions>
                    <v-spacer></v-spacer>

                    <v-btn class="mr-2" variant="outlined" color="primary" @click="closeDialog">
                      <v-icon left class="mr-2">{{ mdiCloseCircle }}</v-icon>
                      Zavřít</v-btn>
                    <v-btn v-if="editedItem.writable" variant="tonal" color="cyan-lighten-4" type="submit"
                      :disabled="isInstallerWritable(editedItem.dsc) && accessLevel != 1">
                      <v-icon left color="warning" class="mr-2">{{ mdiAlert }}</v-icon>
                      Uložit</v-btn>

                  </v-card-actions>
                </v-card>
              </v-form>
            </v-dialog>
          </v-window-item>

          <v-window-item value="info">
            <v-list lines="two" item-props>
              <v-list-item>
                <template v-slot:prepend>
                  <v-avatar :color="selectedDevice.on ? 'green' : 'red'">
                    <v-icon>{{ mdiConnection }}</v-icon>
                  </v-avatar>
                </template>

                <v-list-item-content>
                  <v-list-item-title class="text-wrap">Status</v-list-item-title>
                  <v-list-item-subtitle>{{ selectedDevice.on ? 'Připojeno' : 'Neodpovídá' }}</v-list-item-subtitle>
                </v-list-item-content>

              </v-list-item>

              <v-divider v-if="'last_com' in selectedDevice" inset></v-divider>
              <v-list-item v-if="'last_com' in selectedDevice">
                <template v-slot:prepend>
                  <v-avatar color="grey-darken-1">
                    <v-icon>{{ mdiMessageTextClockOutline }}</v-icon>
                  </v-avatar>
                </template>

                <template v-slot:append>
                  <v-btn @click="commPeriod = selectedDevice.com; commDialog = true;" variant="outlined"
                    color="primary">
                    <v-icon left class="mr-2">{{ mdiTimerSyncOutline }}</v-icon>
                    Změnit
                  </v-btn>
                </template>

                <v-list-item-content>
                  <v-list-item-title class="text-wrap">Poslední komunikace</v-list-item-title>
                  <v-list-item-subtitle>Před {{ lastCommunicationTime }}</v-list-item-subtitle>
                </v-list-item-content>

              </v-list-item>
              <v-divider inset></v-divider>

              <v-list-item>
                <template v-slot:prepend>
                  <v-avatar color="grey-darken-1">
                    <v-icon>{{ mdiWrench }}</v-icon>
                  </v-avatar>
                </template>

                <v-list-item-content>
                  <v-list-item-title class="text-wrap">Verze firmware</v-list-item-title>
                  <v-list-item-subtitle>{{ selectedDevice.fw }}</v-list-item-subtitle>
                </v-list-item-content>

              </v-list-item>
              <v-divider inset></v-divider>

              <v-list-item>
                <template v-slot:prepend>
                  <v-avatar color="grey-darken-1">
                    <v-icon>{{ mdiHubOutline }}</v-icon>
                  </v-avatar>
                </template>

                <v-list-item-content>
                  <v-list-item-title class="text-wrap">MAC Adresa</v-list-item-title>
                  <v-list-item-subtitle>{{ selectedDevice.mac }}</v-list-item-subtitle>
                </v-list-item-content>

              </v-list-item>

              <v-divider inset></v-divider>

              <v-list-item>
                <template v-slot:prepend>
                  <v-avatar color="grey-darken-1">
                    <v-icon>{{ mdiDevices }}</v-icon>
                  </v-avatar>
                </template>

                <v-list-item-content>
                  <v-list-item-title class="text-wrap">Typ zařízení</v-list-item-title>
                  <v-list-item-subtitle>{{ formatDeviceType(selectedDevice.type) }}</v-list-item-subtitle>
                </v-list-item-content>

              </v-list-item>
              <v-divider inset></v-divider>


            </v-list>

            <v-row class="mt-6" justify="end">
              <v-col cols="auto">
                <v-btn @click="deleteDevice" variant="outlined" color="red">
                  <v-icon left class="mr-2">{{ mdiTrashCanOutline }}</v-icon>
                  Odstranit zařízení
                </v-btn>
              </v-col>
            </v-row>


            <!-- Communication period dialog -->
            <v-dialog v-model="commDialog" persistent max-width="500px">
              <v-form v-model="isFormValid" @submit.prevent="savePeriod">
                <v-card>
                  <v-card-title>
                    Změnit periodu komunikace
                  </v-card-title>
                  <v-divider></v-divider>
                  <v-card-text>
                    <v-container>
                      <v-row>
                        <v-col cols="12">
                          <v-text-field v-model="commPeriod" variant="outlined" type="number" min="2" max="3600"
                            step="1" label="Perioda komunikace" :rules="commRules()"></v-text-field>
                        </v-col>
                      </v-row>
                    </v-container>
                  </v-card-text>
                  <v-divider></v-divider>
                  <v-card-actions>
                    <v-spacer></v-spacer>

                    <v-btn class="mr-2" variant="outlined" color="primary" @click="commDialog = false">
                      <v-icon left class="mr-2">{{ mdiCloseCircle }}</v-icon>
                      Zavřít</v-btn>
                    <v-btn variant="tonal" color="cyan-lighten-4" type="submit">
                      <v-icon left color="warning" class="mr-2">{{ mdiAlert }}</v-icon>
                      Uložit</v-btn>

                  </v-card-actions>
                </v-card>
              </v-form>
            </v-dialog>

          </v-window-item>

          <v-window-item value="log">

            <v-data-table-server v-if="deviceTab && deviceTab == 'log'" v-model:items-per-page="logsPerPage"
              :headers="log_headers" :items="logItems" :loading="device_loading" :items-length="totalLogs"
              @update:options="loadLogs">

              <template v-slot:item.msg="{ item }">
                <v-tooltip>
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props" :color="getColor(item.v)" class="white--text"
                      v-if="(item.v !== undefined) && (item.v !== 0)">
                      <v-icon left>
                        {{ getIcon(item.v) }}
                      </v-icon>
                    </v-chip>
                  </template>
                  {{ getText(item.v) }}
                </v-tooltip>
                {{ item.msg }}
              </template>

              <template v-slot:item.t="{ value }">
                {{ formatTimestamp(value) }}
              </template>
            </v-data-table-server>

          </v-window-item>

          <v-window-item value="update">
            <div class="text-caption text-medium-emphasis ml-2 mb-4">
              <p class="text-yellow-darken-3">Nahrajte níže nový firmware (firmware.bin) nebo souborový systém
                (littlefs.bin). K aktualizaci dojde při příštím probuzení zařízení.</p>
            </div>
            <v-file-input show-size v-model="selectedFile" label="File input" variant="outlined"
              accept=".bin"></v-file-input>

            <v-btn @click="uploadFirmware" :disabled="!selectedFile" class="ml-10 mt-2" variant="outlined"
              color="primary">
              <v-icon left class="mr-2">{{ mdiFileUpload }}</v-icon>
              Nahrát
            </v-btn>

          </v-window-item>


          <v-dialog v-model="updateDialog" persistent max-width="600px">
            <v-card>
              <v-card-title>Nahrávání...</v-card-title>
              <v-divider></v-divider>

              <v-card-text class="mb-2">

                <v-progress-linear v-model="uploadProgress" color="primary" height="25">
                  <template v-slot:default="{ value }">
                    <strong>{{ Math.ceil(value) }}%</strong>
                  </template>
                </v-progress-linear>

                <!-- <v-progress-linear :value="uploadProgress" color="primary" striped></v-progress-linear> -->
              </v-card-text>
            </v-card>
          </v-dialog>

        </v-window>
      </v-card-text>

    </v-card>
  </v-navigation-drawer>
</template>

<script>
import baseParametersMixin from '@/mixins/baseParametersMixin';
import axios from 'axios';
import FeederDevice from '@/components/FeederDevice.vue';
import CameraDevice from '@/components/CameraDevice.vue';
import EggCameraDevice from '@/components/EggCameraDevice.vue';
import { deviceEnums } from '@/utils/deviceEnums';

export default {
  mixins: [baseParametersMixin],
  name: 'DevicesView',
  components: {
    FeederDevice, CameraDevice, EggCameraDevice
  },
  data() {
    return {
      ParDscr: {
        Par_R: 1,
        Par_W: 2,
        Par_RW: 3, // Par_R | Par_W = 1 | 2 = 3
        ParNv: 4,
        ParFun: 8,
        ParAccess: 0x0F,
        Par_Public: 0x10,
        Par_Installer: 0x20,
        Par_ESPNow: 0x40,
        Par_U16: 0x100,
        Par_S16: 0x200,
        Par_U32: 0x300,
        Par_S32: 0x400,
        Par_STRING: 0x500,
      },

      ParAtr: {
        BOOL_FLAG: 0x01,
        COMM_PERIOD_FLAG: 0x02,
        CHART_FLAG: 0x04,
        COMMAND_FLAG: 0x10,
        STATE_FLAG: 0x20
      },

      selectedDeviceIndex: null,
      tempDeviceName: '',
      fetchPeriod: 2500,
      devices: [
        // Example devices
        { mac: '00:16:17:e1:28:3f', pair: 2, type: 1, name: 'Krmitko', on: true, last_com: 4, fw: 11, com: 30, sleep: 50 },
        { mac: '00:16:17:e1:28:3f', pair: 2, type: 2, name: 'Krmitko', on: true, last_com: 4, fw: 11, com: 30, sleep: 50 },
        { mac: '00:16:17:e1:27:3f', pair: 2, type: 3, name: 'Vejce', on: true, last_com: 4, fw: 11, com: 30, sleep: 50 },
        // { mac: '00:16:17:e1:28:3f', pair: 2, type: 1, name: 'Krmitko', on: false },
        // { mac: '00:16:17:e1:28:6f', pair: 1, type: 1, name: 'Krmitko', on: false },
        // { mac: '00:16:17:e1:28:5f', pair: 0, type: 1, name: 'Krmitko', on: false },
      ],
      devDrawer: false,
      selectedDevice: null,
      deviceTab: null,
      pairDialog: false,
      parameterHeaders: [
        { title: 'Název', key: 'str' },
        { title: 'Hodnota', align: 'end', key: 'val' },
        { title: '', key: 'writable', align: 'end', width: '1%' },
      ],
      parameters: [
      ],
      refreshCounter: 0,
      editDialog: false,
      editedItem: {},
      commDialog: false,
      commPeriod: 30,
      selectedFile: null,
      updateDialog: false,
      uploadProgress: 0,
      device_loading: true,
      sleepTime: 0,
      sleepIntervalId: null,
      logsPerPage: 10,
      logItems: [],
      totalLogs: 0,
      log_headers: [
        { title: 'Čas', key: 't', align: 'start', sortable: false },
        { title: 'Zpráva', key: 'msg', align: 'start', sortable: false },
      ],
      cameraTimestamps: {}, // Object to store timestamps for each camera
    };
  },
  computed: {
    chartValue: {
      get() {
        if (this.editedItem.str == "NapetiBaterie_mV") {
          return this.editedItem.val.curr & 0x7FFF;
        }
        return this.editedItem.val.curr
      },
      set(newValue) {
        this.editedItem.val.curr = newValue;
      }
    },
    sleepProgress() {
      return (this.sleepTime / this.selectedDevice.com) * 100;
    },
    currentDeviceComponent() {
      const componentMap = {
        1: 'FeederDevice',
        2: 'CameraDevice',
        3: 'EggCameraDevice',
      };
      return componentMap[this.selectedDevice.type] || null;
    },
    currentDeviceEnums() {
      return deviceEnums[this.selectedDevice.type] || {};
    },
    lastCommunicationTime() {
      const seconds = this.selectedDevice.last_com;
      if (seconds >= 60) {
        const minutes = Math.floor(seconds / 60);
        return `${minutes} min`;
      } else {
        return `${seconds} sec`;
      }
    },
  },
  methods: {
    fetchDevices(error_notify = true) {
      axios.get(`/api/get_devices`)
        .then(response => {
          if (response.headers['content-type'].includes('application/json')) {
            this.loading = false;

            const devices = response.data.devices;

            // Check if a device is selected and update its data
            if (this.selectedDevice) {
              const updatedDevice = devices.find(device => device.mac === this.selectedDevice.mac);
              if (updatedDevice) {
                let pairing = this.selectedDevice.pair == '1';
                this.selectedDevice = JSON.parse(JSON.stringify(updatedDevice));

                if (pairing && this.selectedDevice.pair == '2') {
                  this.pairDialog = false;
                  this.$toast.success('Zařízení bylo úspěšně spárováno.');
                }
              }
            }
            this.updateCountdown();

            this.devices = devices;
            this.fetchDataCallback(true);

            if (!this.devDrawer) {
              this.devices.forEach((device, index) => {
                if (device.type == 2 || device.type == 3) {
                  this.fetchCameraTimestamp(index); // Fetch timestamp for each camera
                }
              });
            }

          } else {
            throw new Error('Invalid response type');
          }
        })
        .catch(error => {
          this.fetchDataCallback(false);
          if (error_notify) {
            console.error('Error loading devices:', error);
            this.$toast.error('Došlo k chybě při načítání zařízení. Zkuste stránku obnovit.');
          }
        });
    },

    // Fetch the latest timestamp for a specific camera (deviceId = device.mac)
    fetchCameraTimestamp(deviceId) {
      axios
        .get(`/api/get_image_timestamp`, { params: { id: deviceId } }) // Pass the camera device ID
        .then((response) => {
          const serverTimestamp = response.data.timestamp * 1000; // Convert from seconds to milliseconds
          this.cameraTimestamps[deviceId] = serverTimestamp; // Directly assign the timestamp in the cameraTimestamps object
        })
        .catch((error) => {
          console.error('Error fetching camera timestamp:', error);
        });
    },


    fetchData(error_notify = true) {
      if (this.devDrawer && this.deviceTab && this.deviceTab == "parameters") {
        this.fetchDeviceParameters(this.selectedDeviceIndex, error_notify);
        this.fetchDevices(error_notify);
      }
      else {
        this.fetchDevices(error_notify);
      }
      this.fetchDataForCurrentView(error_notify);
    },

    fetchDeviceParameters(deviceId, error_notify = true) {
      axios.get(`/api/get_device_all_params?id=${deviceId}`)
        .then((response) => {
          this.device_loading = false;

          // const params = JSON.parse(`[{"min":0,"max":11,"dsc":337,"adr":2,"atr":32,"str":"StavKrmitka","val":2},{"min":0,"max":2,"dsc":347,"adr":3,"atr":16,"str":"ManualniOvladani","val":0},{"min":0,"max":2,"dsc":339,"adr":4,"atr":0,"str":"KalibracePrazdne","val":1},{"min":0,"max":2,"dsc":339,"adr":5,"atr":0,"str":"KalibracePlne","val":0},{"min":0,"max":100,"dsc":337,"adr":6,"atr":0,"str":"ChybovyKod","val":0},{"min":-1000000,"max":10000000,"dsc":1105,"adr":7,"atr":0,"str":"AktualniVaha","val":-7562},{"min":1,"max":100,"dsc":337,"adr":9,"atr":4,"str":"AktualniVaha_proc","val":{"vals":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"curr":0,"mins":1}},{"min":0,"max":6,"dsc":1369,"adr":10,"atr":0,"str":"PosledniCasOtevreni","val":"--:--"},{"min":0,"max":6,"dsc":1369,"adr":13,"atr":0,"str":"PosledniCasZavreni","val":"--:--"},{"min":0,"max":6,"dsc":1369,"adr":16,"atr":0,"str":"CasOtevreni","val":"06:22"},{"min":0,"max":6,"dsc":1369,"adr":19,"atr":0,"str":"CasZavreni","val":"19:35"},{"min":5,"max":300,"dsc":337,"adr":22,"atr":4,"str":"NapetiBaterie_mV","val":{"vals":[3253,3253,3253,3253,3253,3253,3253,3253,3253,3253,3253,3253,3253,3253,3253,3253,3253],"curr":-28700,"mins":5}},{"min":0,"max":100,"dsc":343,"adr":23,"atr":0,"str":"UrovenDoplneni_proc","val":40},{"min":0,"max":1,"dsc":343,"adr":24,"atr":1,"str":"AutomatikaOtevreni","val":true},{"min":0,"max":1,"dsc":343,"adr":25,"atr":1,"str":"AutomatikaZavreni","val":true},{"min":-180,"max":180,"dsc":599,"adr":26,"atr":0,"str":"ZpozdeniOtevreni","val":0},{"min":-180,"max":180,"dsc":599,"adr":27,"atr":0,"str":"ZpozdeniZavreni","val":0},{"min":10,"max":1000,"dsc":615,"adr":28,"atr":0,"str":"MotorMaxProud_mA","val":90},{"min":0,"max":1000000,"dsc":1127,"adr":29,"atr":0,"str":"VahaPrazdne","val":608704},{"min":0,"max":2000000,"dsc":1127,"adr":31,"atr":0,"str":"VahaPlne","val":0},{"min":2,"max":3600,"dsc":343,"adr":33,"atr":2,"str":"PeriodaKomunikace_S","val":10},{"min":18,"max":65535,"dsc":337,"adr":34,"atr":8,"str":"VerzeFW","val":18},{"min":0,"max":1,"dsc":355,"adr":35,"atr":1,"str":"RestartCmd","val":false}]`);
          const params = response.data.params;
          this.parameters = params.map(param => ({
            ...param,
            writable: (param.dsc & (this.ParDscr.Par_W)) != 0,
            type: this.getParameterType(param.dsc),
          }));

          // console.log(this.parameters);
        })
        .catch((error) => {
          if (error_notify) {
            console.error('Error fetching device parameters:', error);
            this.$toast.error('Došlo k chybě při načítání parametrů zařízení. Zkuste to znovu později.');
          }
        });
    },

    getParameterType(dsc) {
      if ((dsc & this.ParDscr.Par_STRING) == this.ParDscr.Par_STRING) return 'STRING';
      if ((dsc & this.ParDscr.Par_U16) == this.ParDscr.Par_U16) return 'UINT16';
      if ((dsc & this.ParDscr.Par_S16) == this.ParDscr.Par_S16) return 'INT16';
      if ((dsc & this.ParDscr.Par_U32) == this.ParDscr.Par_U32) return 'UINT32';
      if ((dsc & this.ParDscr.Par_S32) == this.ParDscr.Par_S32) return 'INT32';
      return 'INT';
      // Add more cases as necessary
    },

    getInputType(type) {
      return type == 'STRING' ? 'text' : 'number';
    },

    selectDevice(index) {
      this.selectedDeviceIndex = index;
      this.selectedDevice = JSON.parse(JSON.stringify(this.devices[index]));
      this.tempDeviceName = this.selectedDevice.name;

      if (this.selectedDevice.pair == '0' || this.selectedDevice.pair == '1') {
        this.pairDialog = true;
      } else {
        // this.fetchDeviceParameters(index);
        this.devDrawer = true;
        this.deviceTab = 'overview';
        this.$nextTick(() => {
          this.tabChanged(this.deviceTab);
        });
        // this.tabChanged(this.deviceTab);
      }
    },
    handleRowClick(event, row) {
      const rowData = row.item;
      this.editedItem = JSON.parse(JSON.stringify(rowData));
      this.editDialog = true;
    },

    closeDialog() {
      this.editDialog = false;
    },
    saveEdit() {
      if (!this.isFormValid) {
        return;
      }
      const payload = {
        id: this.selectedDeviceIndex,
        parameters: [
          {
            adr: this.editedItem.adr,
            val: this.editedItem.val,
          },
        ],
      };

      axios.post('/api/set_device_params', payload, {
        headers: {
          'Content-Type': 'application/json',
        },
      })
        .then(response => {
          this.$toast.success('Parameter byl úspěšně změněn.');
          this.editDialog = false;
          this.fetchData();
        })
        .catch(error => {
          console.error('Error updating parameter(s):', error);
          this.$toast.error('Došlo k chybě při změně hodnoty parametru. Zkuste to prosím znovu později.');
        });
    },

    savePeriod() {
      if (!this.isFormValid) {
        return;
      }
      const payload = {
        id: this.selectedDeviceIndex,
        parameters: [
          {
            name: "PeriodaKomunikace_S",
            val: this.commPeriod,
          },
        ],
      };

      axios.post('/api/set_device_params', payload, {
        headers: {
          'Content-Type': 'application/json',
        },
      })
        .then(response => {
          this.$toast.success('Parameter byl úspěšně změněn.');
          this.commDialog = false;
          this.fetchData();
        })
        .catch(error => {
          console.error('Error updating parameter(s):', error);
          this.$toast.error('Došlo k chybě při změně hodnoty parametru. Zkuste to prosím znovu později.');
        });
    },

    pairDevice() {
      const payload = {
        id: this.selectedDeviceIndex,
        name: this.tempDeviceName,
      };

      axios.post('/api/pair_device', payload)
        .then(response => {
          // this.$toast.success('Parametry byly úspěšně uloženy.');
          this.fetchData();

        })
        .catch(error => {
          this.$toast.error('Během párování zařízení došlo k chybě. Zkuste to znovu později.');
        });

    },

    deleteDevice() {
      const deviceId = this.selectedDeviceIndex;
      const url = `/api/delete_device?id=${deviceId}`;

      axios.get(url)
        .then(response => {
          console.log('Device restart request successful:', response);
          this.$toast.success('Zařízení bylo odstraněno.');
        })
        .catch(error => {
          console.error('Error sending device restart request:', error);
          this.$toast.error('Došlo k chybě při odesílání požadavku. Zkuste to znovu později.');
        });

      this.devDrawer = false;
    },

    loadLogs({ page, itemsPerPage, sortBy }) {
      this.device_loading = true;
      const pos = (page - 1) * itemsPerPage;
      const nmr = itemsPerPage;
      const id = this.selectedDeviceIndex;

      let sortParam = sortBy && sortBy.length ? sortBy[0] : {};

      axios.get('/api/get_device_log', {
        params: { pos, nmr, id },
      })
        .then(response => {
          this.logItems = response.data.logs;
          this.totalLogs = response.data.total;
          this.device_loading = false;
        })
        .catch(error => {
          console.error('Error loading logs:', error);
          this.$toast.error('Došlo k chybě při načítání logů. Zkuste to znovu později.');
          this.device_loading = false;
        });
    },

    uploadFirmware() {
      if (!this.selectedFile) {
        alert('Please select a file to upload.');
        return;
      }

      const deviceId = this.selectedDeviceIndex;
      const formData = new FormData();
      formData.append('file', this.selectedFile[0]);

      this.updateDialog = true;

      axios.post(`/api/device_fw_update?id=${deviceId}`, formData, {
        headers: {
          'Content-Type': 'multipart/form-data',
        },
        onUploadProgress: (progressEvent) => {
          const progress = parseInt(Math.round((progressEvent.loaded * 100) / progressEvent.total));
          this.uploadProgress = progress;
        },
      })
        .then(response => {
          this.updateDialog = false;
          this.$toast.success('Soubor byl úspěšně nahrán.');
          this.uploadProgress = 0;
        })
        .catch(error => {
          this.updateDialog = false;
          console.error('Upload failed:', error);
          this.$toast.error('Při nahrávání souboru došlo k chybě. Prosím, zkuste to znovu později.');
          this.uploadProgress = 0;
        });


    },

    valueRules() {
      if (this.editedItem.type == 'STRING') {
        return [
          v => (v.length <= this.editedItem.max) || `Délka musí být maximálně ${this.editedItem.max} znaků.`,
        ];
      } else {
        return [
          v => !isNaN(parseFloat(v)) && isFinite(v) || 'Hodnota musí být číslo.',
          v => (v >= this.editedItem.min) || `Hodnota musí být alespoň ${this.editedItem.min}.`,
          v => (v <= this.editedItem.max) || `Hodnota nesmí být více než ${this.editedItem.max}.`,
        ];
      }
    },

    commRules() {
      return [
        v => !isNaN(parseFloat(v)) && isFinite(v) || 'Hodnota musí být číslo.',
        v => (v >= 2) || `Perioda musí být alespoň ${2}.`,
        v => (v <= 3600) || `Perioda nesmí být více než ${3600}.`,
      ];
    },

    updateCountdown() {
      if (this.sleepIntervalId) {
        clearInterval(this.sleepIntervalId);
      }
      if (this.selectedDevice && this.devDrawer) {
        this.sleepTime = this.selectedDevice.sleep;
      }
      this.sleepIntervalId = setInterval(() => {
        if (!this.selectedDevice || !this.devDrawer) {
          clearInterval(this.sleepIntervalId);
        }

        if (this.sleepTime > 0) {
          this.sleepTime -= 1;
        } else {
          clearInterval(this.sleepIntervalId);
        }
      }, 1000);
    },

    closePairDialog() {
      this.pairDialog = false;
    },
    formatDeviceType(type) {
      return '0x' + type.toString(16).toUpperCase();
    },

    isInstallerWritable(dsc) {
      return (dsc & this.ParDscr.Par_Installer) != 0;
    },

    getEnumData(item) {
      if (!this.isEnumParameter(item)) {
        let displayText;
        if (item.atr & this.ParAtr.CHART_FLAG) {
          displayText = (item.val.curr & 0x7FFF).toString();
        }
        else {
          displayText = item.val.toString();
        }

        return { text: displayText, options: [] };
      }

      let options = [];
      const rangeOptions = Array.from({ length: item.max - item.min + 1 }, (_, i) => ({
        value: i + item.min,
        text: (i + item.min).toString(),
      }));

      if (item.atr & this.ParAtr.COMMAND_FLAG) {
        options = this.currentDeviceEnums.commandFlags;
      } else if (item.atr & this.ParAtr.STATE_FLAG) {
        options = this.currentDeviceEnums.stateFlags;
      }
      else if (item.atr & this.ParAtr.BOOL_FLAG) {
        options = [
          { value: false, text: 'Vypnuto' },
          { value: true, text: 'Zapnuto' },
        ];
      }

      const combinedOptions = (item.atr & this.ParAtr.BOOL_FLAG) ? options : rangeOptions.map(defaultOption => {
        const specificOption = options.find(opt => opt.value === defaultOption.value);
        return specificOption || defaultOption;
      });

      let displayText;
      if (item.atr & this.ParAtr.CHART_FLAG) {
        displayText = (item.val.curr & 0x7FFF).toString();
      }
      else {
        displayText = combinedOptions.find(option => option.value === item.val)?.text || item.val.toString();
      }

      return { text: displayText, options: combinedOptions };
    },

    isEnumParameter(item) {
      return item.atr & this.ParAtr.COMMAND_FLAG || item.atr & this.ParAtr.STATE_FLAG || item.atr & this.ParAtr.BOOL_FLAG;
    },

    getColor(v) {
      switch (v) {
        case 0: return 'grey'; // v_empty
        case 1: return 'red'; // v_error
        case 2: return 'orange'; // v_warning
        case 3: return 'blue'; // v_info
        default: return 'grey';
      }
    },
    getIcon(v) {
      switch (v) {
        case 0: return this.mdiInformationOutline;
        case 1: return this.mdiAlertCircle;
        case 2: return this.mdiAlert;
        case 3: return this.mdiInformationOutline;
        default: return this.mdiInformationOutline;
      }
    },
    getText(v) {
      switch (v) {
        case 0: return '';
        case 1: return ' Error';
        case 2: return ' Warning';
        case 3: return ' Info';
        default: return ' Unknown';
      }
    },
    formatTimestamp(unixTimestamp) {
      const date = new Date(unixTimestamp * 1000);
      const day = date.getDate().toString().padStart(2, '0');
      const month = (date.getMonth() + 1).toString().padStart(2, '0');
      const year = date.getFullYear().toString().slice(-2);
      const hours = date.getHours().toString().padStart(2, '0');
      const minutes = date.getMinutes().toString().padStart(2, '0');
      const seconds = date.getSeconds().toString().padStart(2, '0');

      return `${day}.${month}.${year} ${hours}:${minutes}:${seconds}`;
    },

    tabChanged(tabValue) {
      this.device_loading = true;
      switch (tabValue) {
        case 'overview':
          this.fetchDataForCurrentView();
          break;
        case 'parameters':
          this.fetchDeviceParameters(this.selectedDeviceIndex, true);
          break;
        case 'log':
          // this.handleLogTab();
          break;
        case 'update':
          break;
        default:
          console.log("Unknown tab selected");
      }
    },
    fetchDataForCurrentView(error_notify = true) {
      if (this.$refs.currentDeviceView && this.$refs.currentDeviceView.fetchData && this.deviceTab == 'overview') {
        this.$refs.currentDeviceView.fetchData(error_notify);
      }
    },
  },
};
</script>


<style scoped>
.device-details-drawer {
  z-index: 1009 !important;
  border-left: 2px solid rgba(var(--v-theme-primary-darken-1), 0.8);
}

.new-device {
  border: 2px solid rgba(var(--v-theme-primary), 0.3) !important;
}

.image-container {
  position: relative;
}

.timestamp-overlay {
  position: absolute;
  bottom: 10px;
  left: 10px;
  background: rgba(0, 0, 0, 0.5);
  color: white;
  padding: 5px;
  font-size: 14px;
}
</style>