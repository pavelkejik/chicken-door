<template>
  <v-card elevation="4">
    <div class="card-title-container ma-4">
      <div class="title-line"></div>
      <v-card-title class="headline text-center">Systémový Log</v-card-title>
      <div class="title-line"></div>
    </div>

    <v-card-text>

    <v-data-table-server v-model:items-per-page="itemsPerPage" :headers="headers" :items="serverItems"
      :loading="logs_loading" :items-length="totalItems" @update:options="loadItems">


      <template v-slot:item.msg="{ item }">
        <v-tooltip>
          <template v-slot:activator="{ props }">
        <v-chip v-bind="props" :color="getColor(item.v)" class="white--text" v-if="(item.v !== undefined) && (item.v !== 0)">
          <v-icon left>
            {{ getIcon(item.v) }}
          </v-icon>
          <!-- {{ getText(item.v) }} -->
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
  </v-card-text>

  </v-card>
</template>



<script>
import baseParametersMixin from '@/mixins/baseParametersMixin';
import axios from 'axios';

export default {
  mixins: [baseParametersMixin],
  name: 'LogsComponent',
  data() {
    return {
      // params: dataFields,

      itemsPerPage: 25,
      serverItems: [],
      totalItems: 0,
      logs_loading: true,
      headers: [
        { title: 'Časové razítko', key: 't', align: 'start', sortable: false },
        { title: 'Zpráva', key: 'msg', align: 'start', sortable: false },
      ],

    };
  },

  methods: {

    loadItems({ page, itemsPerPage, sortBy }) {
      this.logs_loading = true;
      const pos = (page - 1) * itemsPerPage;
      const nmr = itemsPerPage;

      let sortParam = sortBy && sortBy.length ? sortBy[0] : {};

      axios.get('/api/get_log', {
        params: { pos, nmr, },
      })
        .then(response => {
          this.serverItems = response.data.logs;
          this.totalItems = response.data.total;
          this.logs_loading = false;
        })
        .catch(error => {
          console.error('Error loading logs:', error);
          this.$toast.error('Error occured during loading logs. Try it again later.');
          this.logs_loading = false;
        });
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
        case 0: return this.mdiInformationOutline; // v_empty
        case 1: return this.mdiAlertCircle; // v_error
        case 2: return this.mdiAlert; // v_warning
        case 3: return this.mdiInformationOutline; // v_info
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

  },
}
</script>
