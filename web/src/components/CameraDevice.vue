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
  </v-row>
</template>

<script>
import axios from 'axios';
import { mdiCameraRetakeOutline } from '@mdi/js';

const dataFields = {
};

export default {
  props: {
    deviceId: {
      type: Number,
      required: true
    }
  },
  data: () => ({
    mdiCameraRetakeOutline,
    loading: false,
    params: dataFields,
    currentImageTimestamp: Date.now(), // Use the current timestamp as default (for initial load)
    displayTimestamp: null, // Display timestamp
  }),
  methods: {
    fetchData() {
      // Fetch the latest timestamp from the server for the specific device
      axios
        .get(`/api/get_image_timestamp`, {
          params: { id: this.deviceId } // Pass the device ID
        })
        .then((response) => {
          const serverTimestamp = response.data.timestamp;

          // Convert the server timestamp to milliseconds if it's in seconds
          const serverTimestampMs = serverTimestamp * 1000; 

          // Compare server timestamp with the current one
          if (serverTimestampMs !== this.currentImageTimestamp) {
            // Update the current timestamp and display timestamp
            this.currentImageTimestamp = serverTimestampMs;
            this.displayTimestamp = new Date(serverTimestampMs).toLocaleString(); // Format for display
          }
        })
        .catch((error) => {
          // this.$toast.error('Došlo k chybě při načítání časového razítka obrázku.');
          console.error("Error fetching the last image timestamp:", error);
        });
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