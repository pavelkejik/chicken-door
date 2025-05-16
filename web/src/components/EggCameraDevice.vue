<template>
  <v-row>
    <v-col cols="12">
      <div class="image-container">

        <v-img :src="`api/get_device_image?id=${deviceId}&timestamp=${currentImageTimestamp}`"></v-img>
        <!-- Show the timestamp overlay -->
        <div v-if="displayTimestamp" class="timestamp-overlay">{{ displayTimestamp }}</div>
      </div>
      <v-btn class="ml-4 mr-4 mt-4" color="primary" variant="tonal" @click="takePhoto()">
        <v-icon left class="mr-1">{{ mdiCameraRetakeOutline }}</v-icon>
        Obnovit
      </v-btn>

    </v-col>

    <v-col cols="12">
      <v-card variant="tonal" color="blue-grey" elevation="4">

        <v-card-title class="d-flex justify-space-between align-center">
          <span class="text-medium-emphasis">Počet vajec</span>
          <v-icon color="primary">{{ mdiEgg }}</v-icon>
        </v-card-title>

        <v-skeleton-loader v-if="loading" type="button"> </v-skeleton-loader>
        <v-card-text v-else class="text-h4 text-left mt-4">
          {{ params.PocetVajec.curr }} ks
        </v-card-text>

        <egg-chart :array-data="eggChartData" :array-labels="eggChartLabels" style="max-height: 200px;"></egg-chart>

      </v-card>
    </v-col>

  </v-row>
</template>

<script>
import axios from 'axios';
import { mdiCameraRetakeOutline, mdiEgg } from '@mdi/js';
import EggChart from '@/components/EggChart.vue';

const dataFields = {
  PocetVajec: { curr: 0, vals: [], mins: 5 }
};

export default {
  props: {
    deviceId: {
      type: Number,
      required: true
    }
  },
  components: {
    EggChart
  },
  data: () => ({
    mdiCameraRetakeOutline,
    mdiEgg,
    loading: true,
    params: dataFields,
    currentImageTimestamp: Date.now(), // Use the current timestamp as default (for initial load)
    displayTimestamp: null, // Display timestamp
  }),
  created() {
    this.loading = true;
  },
  computed: {
    eggChartData() {
      return this.params.PocetVajec.vals;
    },
    eggChartLabels() {
      if (this.params.PocetVajec.vals) {
        return this.generateTimeLabels(this.params.PocetVajec.vals.length, this.params.PocetVajec.mins * 60 * 1000);
      }
      return null;
    }
  },
  methods: {
fetchData(error_notify = true) {
      this.loading = true;

      // 1) get params (PocetVajec.*)
      const p1 = axios.get('/api/get_device_params', {
        params: { id: this.deviceId, PocetVajec: '' }
      });
      // 2) get image timestamp
      const p2 = axios.get('/api/get_image_timestamp', {
        params: { id: this.deviceId }
      });

      Promise.all([p1, p2])
        .then(([r1, r2]) => {
          // update egg params
          this.params = r1.data;
          // update timestamp if changed
          const ts = r2.data.timestamp * 1000;
          if (ts !== this.currentImageTimestamp) {
            this.currentImageTimestamp = ts;
            this.displayTimestamp = new Date(ts).toLocaleString('cs-CZ');
          }
        })
        .catch(err => {
          console.error('CameraEggCard.fetchData error', err);
          if (error_notify) {
            this.$toast.error('Chyba při načítání dat z kamery.');
          }
        })
        .finally(() => {
          this.loading = false;
        });
    },
    generateTimeLabels(dataLength, step) {
      const labels = [];
      const now = new Date();
      for (let i = 0; i < dataLength; i++) {
        const time = new Date(now.getTime() - i * step);
        labels.push(time.toLocaleTimeString('cs-CZ', { hour: '2-digit', minute: '2-digit' }));
      }
      return labels;
    },
    onImageLoad() {
      this.loading = false; // Stop loading once the image is loaded
    },

    onImageError() {
      this.$toast.error('Došlo k chybě při načítání obrázku.');
      this.loading = false;
    },

    takePhoto() {
      const payload = {
        id: this.deviceId,
        parameters: [
          {
            name: "PoriditSnimek",
            val: true
          }
        ]
      };
      axios
        .post("/api/set_device_params", payload, {
          headers: {
            "Content-Type": "application/json"
          }
        })
        .then((response) => {
          this.$toast.success(
            "Snímek se obnoví při příštím probuzení kamery."
          );
        })
        .catch((error) => {
          console.error("Error updating parameter(s):", error);
          this.$toast.error(
            "Došlo k chybě při odesílání požadavku. Zkuste to prosím znovu později."
          );
        });
    }
  }
};
</script>

<style scoped>
.image-container {
  position: relative;
  /* display: inline-block; */
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