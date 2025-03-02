// accessLevelMixin.js
import { defineComponent } from 'vue';
import { accessLevel } from '@/utils/store';

export default defineComponent({
  computed: {
    accessLevel() {
      return accessLevel.value;
    }
  }
});
