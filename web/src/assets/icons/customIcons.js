import { h } from 'vue';
import settingsIcon from './settings.vue';
import menuIcon from './menu.vue';

const customSvgNameToComponent = {
  settingsIcon,
  menuIcon,
};

const customIcons = {
  component: (props) => h(customSvgNameToComponent[props.icon])
};

export { customIcons };