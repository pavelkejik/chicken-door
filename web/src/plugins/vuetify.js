/**
 * plugins/vuetify.js
 *
 * Framework documentation: https://vuetifyjs.com`
 */

// Styles

// import { mdi } from "vuetify/iconsets/mdi";
import { customIcons } from "@/assets/icons/customIcons";
// import { mdi } from '@mdi/js'; // Import the icons you need
import { aliases, mdi } from 'vuetify/iconsets/mdi-svg' // <---- use SVG icons
import 'vuetify/styles'


// Composables
import { createVuetify } from 'vuetify'

const custom = {
  component: (props) =>
    h(props.tag, [h(customSvgNameToComponent[props.icon], { class: 'v-icon__svg' })]),
}

const savedTheme = localStorage.getItem('themePreference') || (window.matchMedia('(prefers-color-scheme: dark)').matches ? 'dark' : 'light');


export default createVuetify({
  theme: {
    defaultTheme: savedTheme, // Default theme
    themes: {
      light: {
        dark: false,
        colors: {
          background: '#f4f5fa',
          customSidebarColor: '#1e293b',
          customTitle: '#0000001f',
        },
      },
      dark: {
        dark: true,
        colors: {
          customSidebarColor: '#212121',
          customTitle: '#fff',
        },
      },
    },
  },
  defaultAssets: false,
  icons: {
    aliases,
    defaultSet: "mdi",
    sets: {
      mdi,
      custom: customIcons,
    },
  },
});
