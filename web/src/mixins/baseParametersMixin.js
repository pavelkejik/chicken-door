import axios from 'axios';
import {
  mdiAlert,
  mdiCloseCircle,
  mdiAlertCircle,
  mdiInformationOutline,
  mdiWrench,
  mdiCellphoneLink,
  mdiTimer,
  mdiChartLineVariant,
  mdiMemory,
  mdiApps,
  mdiSd,
  mdiApplicationBrackets,
  mdiFolder,
  mdiWifiCheck,
  mdiWifiSettings,
  mdiWifiAlert,
  mdiWifiOff,
  mdiWifiStrength1,
  mdiWifiStrength2,
  mdiWifiStrength3,
  mdiWifiStrength4,
  mdiHubOutline,
  mdiRouterNetwork,
  mdiServer,
  mdiAccessPointCheck,
  mdiAccessPointOff,
  mdiDevices,
  mdiWifiRefresh,
  mdiUpdate,
  mdiWeatherSunsetUp,
  mdiWeatherSunsetDown,
  mdiFileUpload,
  mdiPower,
  mdiWhiteBalanceSunny,
  mdiBrightness6,
  mdiSunClock,
  mdiRayStartArrow,
  mdiArrowUpBold,
  mdiArrowDownBold,
  mdiArrowCollapseUp,
  mdiTransferUp,
  mdiWindowShutterOpen,
  mdiWindowShutter,
  mdiHomeThermometerOutline,
  mdiWindowShutterAuto,
  mdiClockPlusOutline,
  mdiPencil,
  mdiPencilLock,
  mdiConnection,
  mdiTrashCanOutline,
  mdiMessageTextClockOutline,
  mdiTimerSyncOutline,
  mdiSleep,
  mdiCamera,
  mdiNetworkOutline,
} from '@mdi/js';
import { pick } from '@/utils/objectHelpers';

export default {
  data() {
    return {
      mdiAlert,
      mdiCloseCircle,
      mdiAlertCircle,
      mdiInformationOutline,
      mdiWrench,
      mdiCellphoneLink,
      mdiTimer,
      mdiChartLineVariant,
      mdiMemory,
      mdiApps,
      mdiSd,
      mdiApplicationBrackets,
      mdiFolder,
      mdiWifiCheck,
      mdiWifiSettings,
      mdiWifiAlert,
      mdiWifiOff,
      mdiWifiStrength1,
      mdiWifiStrength2,
      mdiWifiStrength3,
      mdiWifiStrength4,
      mdiHubOutline,
      mdiRouterNetwork,
      mdiServer,
      mdiAccessPointCheck,
      mdiAccessPointOff,
      mdiDevices,
      mdiWifiRefresh,
      mdiUpdate,
      mdiWeatherSunsetUp,
      mdiWeatherSunsetDown,
      mdiFileUpload,
      mdiPower,
      mdiWhiteBalanceSunny ,
      mdiBrightness6,
      mdiSunClock,
      mdiRayStartArrow,
      mdiArrowUpBold,
      mdiArrowDownBold,
      mdiArrowCollapseUp,
      mdiTransferUp ,
      mdiWindowShutterOpen,
      mdiWindowShutter,
      mdiHomeThermometerOutline,
      mdiWindowShutterAuto,
      mdiClockPlusOutline,
      mdiPencil,
      mdiConnection,
      mdiPencilLock,
      mdiTrashCanOutline,
      mdiMessageTextClockOutline,
      mdiTimerSyncOutline,
      mdiSleep,
      mdiCamera,
      mdiNetworkOutline,
      loading: true,
      isFormValid: false,
      shouldContinueFetching: false,
      fetchTimeout: null,
      fetchPeriod: 0,
      params: {},
      originalParams: {},
      rules: {
        required: value => !!value || 'Toto pole je povinné.',
        ipAddress: value => {
          const pattern = /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$|^([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}$/;
          return pattern.test(value) || 'Nevalidní IPV4 adresa';
        },
      },
      // params and originalParams will be initialized in the component
    };
  },
  computed: {
    hasChanges() {
      return JSON.stringify(this.params) !== JSON.stringify(this.originalParams);
    },
  },
  methods: {
    fetchData(error_notify = true) {
      if (Object.keys(this.params).length === 0) {
        return;
      }
      let queryParams = new URLSearchParams(this.params).toString();
      axios.get(`/api/get_params?${queryParams}`)
        .then(response => {
          if (response.headers['content-type'].includes('application/json')) {
            this.loading = false;
            this.params = response.data;
            this.originalParams = JSON.parse(JSON.stringify(response.data));
            this.fetchDataCallback(true);
          } else {
            throw new Error('Invalid response type');
          }
        })
        .catch(error => {
          this.fetchDataCallback(false);
          if (error_notify) {
            console.error('Error loading parameters:', error);
            this.$toast.error('Došlo k chybě při načítání parametrů. Zkuste stránku obnovit.');
          }
        });
    },
    submitData() {
      if (!this.isFormValid) {
        return;
      }

      axios.post('/api/set_params', this.params)
        .then(response => {
          this.$toast.success('Parametry byly úspěšně uloženy.');
          this.fetchData();

        })
        .catch(error => {
          this.$toast.error('Během aktualizace parametrů došlo k chybě. Zkuste to znovu později.');

        });
    },
    submitDataList(keys, success_print = true) {
      const formData = pick(this.params, keys);

      axios.post('/api/set_params', formData)
        .then(response => {
          if (success_print) {
            this.$toast.success('Parametry byly úspěšně uloženy.');
          }
          this.fetchData();

        })
        .catch(error => {
          this.$toast.error('Během aktualizace parametrů došlo k chybě. Zkuste to znovu později.');

        });
    },
    resetSettings() {
      this.params = JSON.parse(JSON.stringify(this.originalParams));
    },


    InitParameters() {
      this.params = JSON.parse(JSON.stringify(this.params));
      this.originalParams = JSON.parse(JSON.stringify(this.params));
    },

    fetchDataCallback(success)
    {
    },
    mountCallback()
    {
    },

    performFetch() {
      if (!this.shouldContinueFetching) return;

      if(this.fetchPeriod > 0)
      {
      this.fetchData(false);
      }
      
      this.fetchTimeout = setTimeout(this.performFetch, this.fetchPeriod > 0? this.fetchPeriod : 1000);
    },

    startFetching() {
      this.shouldContinueFetching = true;
      this.fetchTimeout = setTimeout(this.performFetch, this.fetchPeriod > 0? this.fetchPeriod : 1000);
    },
    stopFetching() {
      this.shouldContinueFetching = false;
      clearTimeout(this.fetchTimeout);
    },
  },
  created() {
    this.InitParameters();
    this.$watch(
      () => this.$route.params,
      () => {
        this.loading = true;
        this.fetchData();
        this.mountCallback();
      },
      { immediate: true }
    );
  },
  mounted() {
      this.startFetching();
  },
  beforeDestroy() {
    this.stopFetching();
  },
  beforeUnmount() {
    this.stopFetching();
  },
};
