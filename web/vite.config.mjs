// Plugins
import Components from 'unplugin-vue-components/vite'
import Vue from '@vitejs/plugin-vue'
import Vuetify, { transformAssetUrls } from 'vite-plugin-vuetify'
import ViteFonts from 'unplugin-fonts/vite'
import Compression from 'vite-plugin-compression'

// Utilities
import { defineConfig } from 'vite'
import { fileURLToPath, URL } from 'node:url'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [
    Vue({
      template: { transformAssetUrls }
    }),
    // https://github.com/vuetifyjs/vuetify-loader/tree/master/packages/vite-plugin#readme
    Vuetify({ autoImport: true, styles: { configFile: 'src/assets/styles/settings.scss' } }),
    Components(),
    Compression({
      verbose: true, // Optional: it logs the compressed files
      disable: false, // Optional: true in dev mode, false in prod mode
      threshold: 1040, // Only compress assets larger than this size in bytes
      algorithm: 'gzip', // The compression algorithm (gzip, brotli, etc.)
      ext: '.gz', // The file extension for the compressed files
    }),
  ],
  define: { 'process.env': {} },
  assetsInclude: ['**/*.png', '**/*.PNG'], // Add this line
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url))
    },
    extensions: [
      '.js',
      '.json',
      '.jsx',
      '.mjs',
      '.ts',
      '.tsx',
      '.vue',
    ],
  },
  server: {
    port: 3000,
  },
})
