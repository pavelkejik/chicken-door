<template>
  <v-row align="start">
    <v-col cols="12" sm="12" md="6" lg="4" xl="3">
      <v-card>
        <v-card-title class="d-flex justify-space-between align-center">
          <span class="text-medium-emphasis">Stav</span>
          <v-icon color="primary">{{ mdiWindowShutter }}</v-icon>
        </v-card-title>

        <v-skeleton-loader class="mb-10" v-if="loading" type="image"> </v-skeleton-loader>
        <v-card-text v-else>
          <v-row align="center">
            <v-col cols="8">
              <svg style="max-height: 300px;" viewBox="7456 9866 8086 9984" xmlns="http://www.w3.org/2000/svg"
                xmlns:xlink="http://www.w3.org/1999/xlink">
                <defs>

                  <filter id="combinedFilter" x="0%" y="0%" width="100%" height="100%">
                    <feTurbulence type="fractalNoise" baseFrequency="0.50" numOctaves="2" stitchTiles="stitch"
                      result="speckles" />
                    <feComponentTransfer in="speckles" result="sharpSpeckles">
                      <feFuncA type="discrete" tableValues="0 0 1" />
                    </feComponentTransfer>
                    <feColorMatrix type="matrix" values="0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0.05 0" in="sharpSpeckles"
                      result="coloredSpeckles" />
                    <feBlend in="SourceGraphic" in2="coloredSpeckles" mode="screen" />
                  </filter>

                  <radialGradient id="door-fill" gradientUnits="userSpaceOnUse" cx="10350" cy="12055" r="11500"
                    xlink:href="#door-fill-grad" />
                  <linearGradient id="door-fill-grad" gradientUnits="userSpaceOnUse">
                    <stop style="stop-color: rgba(30, 33, 87, 0.956);" offset="0" />
                    <stop style="stop-color: rgba(57, 65, 183, 0.963);" offset="0.2" />
                  </linearGradient>

                </defs>
                <path stroke-width="10" fill="none" stroke="rgb(81,81,81)" d="M 13540,14239 L 13540,11889 13540,11089" />
                <path stroke-width="10" fill="none" stroke="rgb(81,81,81)" d="M 13411,11889 L 13411,13922" />
                <path stroke-width="10" fill="none" stroke="rgb(81,81,81)" d="M 13411,14239 L 13540,14239" />
                <path stroke-width="10" fill="none" stroke="rgb(81,81,81)" d="M 13540,11889 L 13411,11889" />
                <path stroke-width="10" fill="none" stroke="rgb(81,81,81)" d="M 12371,11089 L 13540,11089" />
                <path style="fill:url('#door-fill');" stroke="none" :d="doorPosition.fill" />
                <path stroke-width="10" fill="none" stroke="rgb(81,81,81)" :d="doorPosition.path" />
                <path fill="none" stroke-width="10" stroke="rgb(81,81,81)" d="M 13411,11889 L 12371,11889" />
                <path fill="none" stroke-width="10" stroke="rgb(81,81,81)" d="M 11726,13095 L 11597,13095" />
                <path fill="rgb(85,85,85)" filter="url(#combinedFilter)" stroke="none"
                  d="M 10499,13095 L 9272,13095 9272,11089 11726,11089 11726,13095 10499,13095 Z" />
                <path fill="none" stroke-width="10" stroke="rgb(81,81,81)"
                  d="M 10499,13095 L 9272,13095 9272,11089 11726,11089 11726,13095 10499,13095 Z" />
                <path fill="none" stroke-width="10" stroke="rgb(81,81,81)"
                  d="M 9142,13224 L 9272,13353 11726,13353 11855,13224" />
                <path fill="none" stroke-width="10" stroke="rgb(81,81,81)"
                  d="M 13411,12102 L 13385,12102 13140,12102 13114,12102 13114,19848 13140,19848 13385,19848 13411,19848 13411,13922" />
                <path fill="rgb(75,75,75)" filter="url(#combinedFilter)" stroke="none"
                  d="M 11726,9867 L 11855,9996 11855,9996 11726,10125 11726,10125 9272,10125 C 9228,10082 9185,10039 9142,9996 9185,9953 9228,9910 9272,9867 L 9272,9867 11726,9867 11726,9867 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 11726,9867 L 11855,9996 11855,9996 11726,10125 11726,10125 9272,10125 C 9228,10082 9185,10039 9142,9996 9185,9953 9228,9910 9272,9867 L 9272,9867 11726,9867 11726,9867 Z" />
                <path fill="rgb(66,66,66)" filter="url(#combinedFilter)" stroke="none"
                  d="M 9272,11081 L 9013,11081 9013,11081 9013,10125 9142,9996 9142,9996 9272,10125 9272,10125 9272,11081 9272,11081 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 9272,11081 L 9013,11081 9013,11081 9013,10125 9142,9996 9142,9996 9272,10125 9272,10125 9272,11081 9272,11081 Z" />
                <path fill="rgb(85,85,85)" filter="url(#combinedFilter)" stroke="none"
                  d="M 10499,11081 L 9272,11081 9272,10125 11726,10125 11726,11081 10499,11081 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 10499,11081 L 9272,11081 9272,10125 11726,10125 11726,11081 10499,11081 Z" />
                <path fill="rgb(66,66,66)" filter="url(#combinedFilter)" stroke="none"
                  d="M 11726,11081 L 11984,11081 11984,11081 11984,10125 11984,10125 11855,9996 11855,9996 11726,10125 11726,10125 11726,11081 11726,11081 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 11726,11081 L 11984,11081 11984,11081 11984,10125 11984,10125 11855,9996 11855,9996 11726,10125 11726,10125 11726,11081 11726,11081 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8626,10203 L 8626,10203 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8497,10332 L 8497,10332 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8626,10203 L 8626,10203 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8497,10332 L 8497,10332 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8626,10203 L 8626,10203 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8884,10203 L 8884,10203 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8884,10203 L 8884,10203 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8884,10203 L 8884,10203 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8884,10203 L 8884,10203 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8884,10203 L 8884,10203 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8884,10203 L 8884,10203 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8626,10203 L 8626,10203 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8626,10203 L 8626,10203 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8626,10203 L 8626,10203 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8497,10332 L 8497,10332 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8497,10332 L 8497,10332 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8497,10332 L 8497,10332 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10" d="M 8497,10874 L 8497,10874 Z" />
                <path fill="rgb(17,17,17)" stroke="none"
                  d="M 11417,12795 L 11320,12795 11320,11254 11514,11254 11514,12795 11417,12795 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 11417,12795 L 11320,12795 11320,11254 11514,11254 11514,12795 11417,12795 Z" />
                <path fill="rgb(8,101,107)" stroke="none"
                  d="M 11416,12795 L 11338,12795 11338,11884 11493,11884 11493,12795 11416,12795 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 11416,12795 L 11338,12795 11338,11884 11493,11884 11493,12795 11416,12795 Z" />
                <path fill="rgb(17,17,17)" stroke="none"
                  d="M 9581,12795 L 9484,12795 9484,11254 9678,11254 9678,12795 9581,12795 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 9581,12795 L 9484,12795 9484,11254 9678,11254 9678,12795 9581,12795 Z" />
                <path fill="rgb(8,101,107)" stroke="none"
                  d="M 9580,12795 L 9502,12795 9502,11884 9657,11884 9657,12795 9580,12795 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 9580,12795 L 9502,12795 9502,11884 9657,11884 9657,12795 9580,12795 Z" />
                <path fill="rgb(66,66,66)" filter="url(#combinedFilter)" stroke="none"
                  d="M 11726,13353 L 11855,13224 11855,13224 11726,13095 11726,13095 9272,13095 C 9228,13138 9185,13181 9142,13224 9185,13267 9228,13310 9272,13353 L 9272,13353 11726,13353 11726,13353 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 11726,13353 L 11855,13224 11855,13224 11726,13095 11726,13095 9272,13095 C 9228,13138 9185,13181 9142,13224 9185,13267 9228,13310 9272,13353 L 9272,13353 11726,13353 11726,13353 Z" />
                <path fill="rgb(66,66,66)" filter="url(#combinedFilter)" stroke="none"
                  d="M 12113,11889 L 12113,11089 12113,11089 11726,11089 11726,11089 11726,13095 11726,13095 11855,13224 11855,13224 11984,13095 11984,13095 11984,11928 11984,11928 12023,11889 12023,11889 12113,11889 12113,11889 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 12113,11889 L 12113,11089 12113,11089 11726,11089 11726,11089 11726,13095 11726,13095 11855,13224 11855,13224 11984,13095 11984,13095 11984,11928 11984,11928 12023,11889 12023,11889 12113,11889 12113,11889 Z" />
                <path fill="rgb(208,208,208)" stroke="none"
                  d="M 12955,11889 L 12370,11889 12370,11089 13539,11089 13539,11889 12955,11889 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 12955,11889 L 12370,11889 12370,11089 13539,11089 13539,11889 12955,11889 Z" />
                <path fill="rgb(85,85,85)" filter="url(#combinedFilter)" stroke="none"
                  d="M 12371,11089 L 12113,11089 12113,11089 12113,11889 12113,11889 12371,11889 12371,11889 12500,11760 12500,11760 12500,11218 12500,11218 12371,11089 12371,11089 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 12371,11089 L 12113,11089 12113,11089 12113,11889 12113,11889 12371,11889 12371,11889 12500,11760 12500,11760 12500,11218 12500,11218 12371,11089 12371,11089 Z" />
                <path fill="rgb(208,208,208)" stroke="none"
                  d="M 13475,14239 L 13410,14239 13410,11889 13539,11889 13539,14239 13475,14239 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 13475,14239 L 13410,14239 13410,11889 13539,11889 13539,14239 13475,14239 Z" />
                <path fill="rgb(153,153,153)" stroke="none"
                  d="M 13262,19848 L 13113,19848 13113,12102 13410,12102 13410,19848 13262,19848 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 13262,19848 L 13113,19848 13113,12102 13410,12102 13410,19848 13262,19848 Z" />
                <path fill="rgb(66,66,66)" filter="url(#combinedFilter)" stroke="none"
                  d="M 8885,11890 L 8885,11090 8885,11090 9272,11090 9272,11090 9272,13096 9272,13096 9143,13225 9143,13225 9014,13096 9014,13096 9014,11929 9014,11929 8975,11890 8975,11890 8885,11890 8885,11890 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 8885,11890 L 8885,11090 8885,11090 9272,11090 9272,11090 9272,13096 9272,13096 9143,13225 9143,13225 9014,13096 9014,13096 9014,11929 9014,11929 8975,11890 8975,11890 8885,11890 8885,11890 Z" />
                <path fill="rgb(208,208,208)" stroke="none"
                  d="M 8042,11890 L 7457,11890 7457,11090 8626,11090 8626,11890 8042,11890 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 8042,11890 L 7457,11890 7457,11090 8626,11090 8626,11890 8042,11890 Z" />
                <path fill="rgb(85,85,85)" filter="url(#combinedFilter)" stroke="none"
                  d="M 8627,11090 L 8885,11090 8885,11090 8885,11890 8885,11890 8627,11890 8627,11890 8498,11761 8498,11761 8498,11219 8498,11219 8627,11090 8627,11090 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 8627,11090 L 8885,11090 8885,11090 8885,11890 8885,11890 8627,11890 8627,11890 8498,11761 8498,11761 8498,11219 8498,11219 8627,11090 8627,11090 Z" />
                <path fill="rgb(208,208,208)" stroke="none"
                  d="M 7522,14240 L 7457,14240 7457,11890 7586,11890 7586,14240 7522,14240 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 7522,14240 L 7457,14240 7457,11890 7586,11890 7586,14240 7522,14240 Z" />
                <path fill="rgb(153,153,153)" stroke="none"
                  d="M 7735,19848 L 7586,19848 7586,12102 7883,12102 7883,19848 7735,19848 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 7735,19848 L 7586,19848 7586,12102 7883,12102 7883,19848 7735,19848 Z" />
                <path fill="rgb(18,111,117)" stroke="none"
                  d="M 9400,13256 C 9400,13269 9404,13282 9410,13294 9417,13306 9427,13316 9439,13323 9451,13329 9464,13333 9478,13333 9478,13333 9478,13333 9478,13333 L 9478,13333 11520,13333 11520,13333 C 11534,13333 11547,13329 11558,13323 11570,13316 11580,13306 11587,13294 11593,13282 11597,13269 11597,13256 L 11597,13256 11597,12867 11597,12867 C 11597,12853 11593,12840 11587,12828 11580,12816 11570,12806 11558,12799 11547,12793 11534,12789 11520,12789 L 11520,12789 9478,12789 9478,12789 C 9478,12789 9478,12789 9478,12789 9464,12789 9451,12793 9439,12799 9427,12806 9417,12816 9410,12828 9404,12840 9400,12853 9400,12867 L 9400,12867 9400,13256 9400,13256 Z" />
                <path fill="none" stroke="rgb(81,81,81)" stroke-width="10"
                  d="M 9400,13256 C 9400,13269 9404,13282 9410,13294 9417,13306 9427,13316 9439,13323 9451,13329 9464,13333 9478,13333 9478,13333 9478,13333 9478,13333 L 9478,13333 11520,13333 11520,13333 C 11534,13333 11547,13329 11558,13323 11570,13316 11580,13306 11587,13294 11593,13282 11597,13269 11597,13256 L 11597,13256 11597,12867 11597,12867 C 11597,12853 11593,12840 11587,12828 11580,12816 11570,12806 11558,12799 11547,12793 11534,12789 11520,12789 L 11520,12789 9478,12789 9478,12789 C 9478,12789 9478,12789 9478,12789 9464,12789 9451,12793 9439,12799 9427,12806 9417,12816 9410,12828 9404,12840 9400,12853 9400,12867 L 9400,12867 9400,13256 9400,13256 Z" />

                <path v-if="params.HorniPoloha == 2" :d="mdiArrowCollapseUp"
                  :transform="`translate(14000, 12828) scale(30.2)`" :fill="'green'" />

                <path v-if="params.DolniPoloha == 2" :d="mdiArrowCollapseUp"
                  :transform="`translate(14720, 19828) rotate(180) scale(30.2)`" :fill="'green'" />

                <path v-if="params.StavDvirka == 1 || params.StavDvirka == 2" :d="mdiTransferUp"
                  :transform="`translate(13730, 15228) scale(55.2)`" :fill="'red'" class="blink" />

                <path v-if="params.StavDvirka == 6 || params.StavDvirka == 7" :d="mdiTransferUp"
                  :transform="`translate(15030, 16828) rotate(180) scale(55.2)`" :fill="'red'" class="blink" />


              </svg>
            </v-col>
            <v-spacer></v-spacer>
            <v-col>
              <v-row>
                <v-col>
                  <v-btn density="default" color="primary" :icon="mdiArrowUpBold" @click="doorCommand(1)"></v-btn>
                </v-col>
              </v-row>
              <v-row class="mt-8">
                <v-col>
                  <v-btn density="default" color="primary" :icon="mdiArrowDownBold" @click="doorCommand(2)"></v-btn>
                </v-col>
              </v-row>

            </v-col>
          </v-row>

        </v-card-text>

        <v-divider></v-divider>

        <v-skeleton-loader class="mb-10 mt-6" v-if="loading" type="list-item, list-item"> </v-skeleton-loader>
        <v-card-text v-else>
          <v-row v-if="params.StavDvirka == 14" no-gutters>
            <v-col>
              <v-sheet class="pa-2 ma-2">
                <v-avatar class="mr-4 blink" color="red">

                  <v-icon size="28">{{ mdiAlertCircle }}</v-icon>
                </v-avatar>

                Chyba dveří!
              </v-sheet>

            </v-col>
          </v-row>

          <v-row no-gutters>
            <v-col cols="8">
              <v-sheet class="pa-2 ma-2">
                <v-icon size="28" class="mr-4">{{ mdiWindowShutterOpen }}</v-icon>
                Naposledy otevřeno v:
              </v-sheet>

            </v-col>
            <v-col>
              <v-sheet class="pa-2 ma-2 text-right">
                {{ params.CasOtevreni }}
              </v-sheet>
            </v-col>
          </v-row>

          <v-row no-gutters>
            <v-col cols="8">
              <v-sheet class="pa-2 ma-2">
                <v-icon size="28" class="mr-4">{{ mdiWindowShutter }}</v-icon>
                Naposledy zavřeno v:
              </v-sheet>
            </v-col>
            <v-col>
              <v-sheet class="pa-2 ma-2 text-right">
                {{ params.CasZavreni }}
              </v-sheet>
            </v-col>
          </v-row>

        </v-card-text>

      </v-card>
    </v-col>
    <v-col cols="12" sm="12" md="6" lg="4" xl="3">
      <v-card>

        <v-card-title class="d-flex justify-space-between align-center">
          <span class="text-medium-emphasis">Denní automatika</span>
          <v-icon color="primary">{{ mdiWindowShutterAuto }}</v-icon>
        </v-card-title>

        <v-skeleton-loader class="mb-10" v-if="loading" type="image"> </v-skeleton-loader>
        <v-card-text v-else>
          <svg viewBox="5597 18008 10173 4647" xmlns="http://www.w3.org/2000/svg"
            xmlns:xlink="http://www.w3.org/1999/xlink">
            <defs class="EmbeddedBulletChars">
              <clipPath id="clip-left">
                <rect x="5597" y="17848" :width="sunPosition.sun.x - 5196" height="5886" />
              </clipPath>

              <radialGradient id="color-day-fill" gradientUnits="userSpaceOnUse" cx="10700" cy="20055" r="3000"
                xlink:href="#color-day-fill-grad" />
              <linearGradient id="color-day-fill-grad" gradientUnits="userSpaceOnUse">
                <stop style="stop-color: rgba(244, 181, 0, 0.763);" offset="0" />
                <stop style="stop-color: rgba(237, 113, 0, 0.257);" offset="1" />
              </linearGradient>

              <linearGradient id="color-day-path" gradientUnits="userSpaceOnUse" x1="15700" y1="19655" x2="15700"
                y2="21555" xlink:href="#color-day-path-grad" />
              <linearGradient id="color-day-path-grad" gradientUnits="userSpaceOnUse">
                <stop offset="0" style="stop-color: rgb(255, 224, 0); " />
                <stop offset="0.36" style="stop-color: rgba(229, 164, 0); " />
              </linearGradient>

              <radialGradient id="color-night-fill" gradientUnits="userSpaceOnUse" cx="10700" cy="21055" r="6000"
                xlink:href="#color-night-fill-grad" />
              <linearGradient id="color-night-fill-grad" gradientUnits="userSpaceOnUse">
                <stop style="stop-color: rgba(154, 199, 207, 0.257);" offset="0" />
                <stop style="stop-color: rgba(42, 96, 153, 0.563);" offset="1" />
              </linearGradient>

              <linearGradient id="color-night-path" gradientUnits="userSpaceOnUse" x1="15700" y1="20655" x2="15700"
                y2="22555" xlink:href="#color-night-path-grad" />
              <linearGradient id="color-night-path-grad" gradientUnits="userSpaceOnUse">
                <title>blue</title>
                <stop offset="0" style="stop-color: rgb(229, 164, 0);" />
                <stop offset="0.34" style="stop-color: rgba(26, 50, 247, 0.55);" />
              </linearGradient>

            </defs>
            <g class="Page">

              <g class="com.sun.star.drawing.ClosedBezierShape">
                <g id="id7">
                  <rect class="BoundingBox" stroke="none" fill="none" x="7847" y="18848" width="5673" height="1886" />
                  <g style="opacity: 0.419607843137255">
                    <g>

                      <g clip-path="url(#clip-left)">
                        <path style="fill:url('#color-day-fill');"
                          d="M 13519,20731 L 13364,20576 13283,20496 13206,20423 13206,20423 13207,20423 12785,20019 12365,19639 12163,19471 12057,19388 11945,19307 11847,19241 11746,19176 11640,19113 11525,19054 11428,19009 11327,18966 11275,18946 11220,18927 11164,18910 11105,18895 11005,18875 10900,18861 10793,18851 10685,18848 10576,18851 10468,18860 10363,18875 10262,18895 10203,18910 10146,18927 10092,18946 10039,18966 9938,19009 9842,19054 9727,19113 9620,19176 9519,19241 9422,19307 9310,19388 9204,19471 9002,19639 8582,20019 8162,20431 8162,20431 7847,20733 7847,20733 13519,20731 Z" />
                      </g>
                    </g>

                  </g>
                </g>
              </g>

              <g class="com.sun.star.drawing.ClosedBezierShape">
                <g id="id8">
                  <rect class="BoundingBox" stroke="none" fill="none" x="5637" y="20733" width="2211" height="1282" />
                  <g>

                    <g clip-path="url(#clip-left)">
                      <path style="fill:url('#color-night-fill')"
                        d="M 7847,20733 L 7739,20834 7534,21029 7429,21126 7319,21222 7117,21387 7011,21468 6899,21547 6801,21613 6700,21678 6593,21741 6479,21800 6383,21845 6281,21887 6174,21925 6118,21942 6059,21958 6009,21969 5958,21977 5852,21991 5637,22014 5637,22014 5637,20733 5641,20733 7847,20733 Z" />
                    </g>
                  </g>
                </g>
              </g>
              <g class="com.sun.star.drawing.ClosedBezierShape">
                <g id="id9">
                  <rect class="BoundingBox" stroke="none" fill="none" x="13541" y="20732" width="2211" height="1282" />
                  <g>
                    <g clip-path="url(#clip-left)">
                      <path style="fill:url('#color-night-fill')"
                        d="M 13541,20732 L 13649,20833 13855,21028 13960,21125 14069,21221 14271,21386 14377,21467 14489,21546 14587,21612 14688,21677 14795,21740 14909,21799 15005,21844 15107,21886 15214,21924 15270,21941 15329,21957 15379,21968 15430,21976 15536,21990 15751,22013 15751,22013 15751,20732 15747,20732 13541,20732 Z" />
                    </g>
                  </g>

                </g>
              </g>
            </g>

            <g class="com.sun.star.drawing.OpenBezierShape">
              <g id="id3">
                <rect class="BoundingBox" stroke="none" fill="none" x="13479" y="20691" width="2291" height="1363" />
                <path fill="none" stroke="url('#color-night-path')" stroke-width="80" stroke-linejoin="round"
                  d="M 15729,22013 C 15588,21995 15437,21989 15307,21958 15148,21919 15013,21859 14887,21799 14727,21723 14596,21635 14466,21546 14313,21442 14177,21332 14046,21222 13896,21095 13763,20964 13626,20834 13581,20787 13565,20776 13519,20731" />
              </g>
            </g>
            <g class="com.sun.star.drawing.OpenBezierShape">
              <g id="id4">
                <rect class="BoundingBox" stroke="none" fill="none" x="7807" y="18808" width="5753" height="1966" />
                <path fill="none" stroke="url('#color-day-path')" stroke-width="80" stroke-linejoin="round"
                  d="M 13519,20731 C 13425,20639 13303,20512 13206,20423 L 13206,20423 13207,20423 C 13067,20288 12928,20153 12785,20019 12648,19892 12512,19764 12365,19639 12231,19527 12099,19413 11945,19307 11816,19219 11684,19130 11525,19054 11398,18994 11265,18933 11105,18895 10975,18866 10829,18848 10685,18848 10540,18848 10392,18865 10262,18895 10101,18932 9968,18994 9842,19054 9682,19130 9550,19219 9422,19307 9268,19413 9135,19527 9002,19639 8855,19764 8718,19891 8582,20019 8437,20155 8304,20294 8162,20431 L 8162,20431 C 8068,20519 7944,20641 7847,20733" />
              </g>
            </g>
            <g class="com.sun.star.drawing.OpenBezierShape">
              <g id="id5">
                <rect class="BoundingBox" stroke="none" fill="none" x="5597" y="20693" width="2291" height="1362" />
                <path fill="none" stroke="url('#color-night-path')" stroke-width="80" stroke-linejoin="round"
                  d="M 7847,20733 C 7803,20775 7786,20788 7739,20834 7601,20964 7469,21095 7319,21222 7188,21333 7052,21443 6899,21547 6769,21635 6638,21724 6479,21800 6353,21860 6218,21919 6059,21958 5929,21989 5778,21995 5637,22014" />
              </g>
            </g>
            <g class="com.sun.star.drawing.LineShape">
              <g id="id6">
                <rect class="BoundingBox" stroke="none" fill="none" x="5623" y="20705" width="10143" height="45" />
                <path fill="none" stroke="rgb(128, 128, 128)" stroke-opacity="0.7" stroke-width="28"
                  stroke-linejoin="round" d="M 5637,20735 L 15751,20719" />
              </g>
            </g>

            <path :d="mdiWhiteBalanceSunny"
              :transform="`translate(${sunPosition.sun.x}, ${sunPosition.sun.y}) scale(30.2)`" fill="orange" />

            <path :d="mdiRayStartArrow"
              :transform="`translate(${sunPosition.close.x + 830}, ${sunPosition.close.y + 230}) rotate(90) scale(37.2)`"
              :fill="'rgba(var(--v-theme-primary),0.8)'" />
            <text :x="sunPosition.close.x - 620" :y="sunPosition.close.y + 1040"
              :style="{ fill: 'rgb(var(--v-theme-primary))' }" font-size="300">{{
                autoCloseTime
              }}</text>

            <path :d="mdiRayStartArrow"
              :transform="`translate(${sunPosition.open.x - 155}, ${sunPosition.open.y + 550}) rotate(-90) scale(37.2)`"
              :fill="'rgba(var(--v-theme-primary),0.8)'" />
            <text :x="sunPosition.open.x - 640" :y="sunPosition.open.y - 100"
              :style="{ fill: 'rgb(var(--v-theme-primary))' }" font-size="300">{{
                autoOpenTime
              }}</text>

            <text x="6997" y="20600" :style="{ fill: 'rgb(var(--v-theme-on-surface))' }" font-size="300">{{
              params.CasVychodu
            }}</text>
            <text x="14521" y="20600" :style="{ fill: 'rgb(var(--v-theme-on-surface))' }" font-size="300"
              text-anchor="end">{{ params.CasZapadu }}</text>


            <path fill="none" :stroke="'rgb(var(--v-theme-primary))'" stroke-opacity="0.4" stroke-width="100"
              :d="`M ${sunPosition.open.x + 800},21862 L ${sunPosition.close.x + -200},21862`" />
            <path :fill="'rgb(var(--v-theme-primary))'" fill-opacity="0.4" stroke="none"
              :d="`M ${sunPosition.open.x + 800},22012 L ${sunPosition.open.x + 500},21862 ${sunPosition.open.x + 800},21712 ${sunPosition.open.x + 800},22012 Z`" />
            <path :fill="'rgb(var(--v-theme-primary))'" fill-opacity="0.4" stroke="none"
              :d="`M ${sunPosition.close.x - 200},21712 L ${sunPosition.close.x + 100},21862 ${sunPosition.close.x - 200},22012 ${sunPosition.close.x - 200},21712 Z`" />
            <text :x="`${(sunPosition.close.x + sunPosition.open.x) / 2 + 900}`" y="21712"
              :style="{ fill: 'rgb(var(--v-theme-on-surface))' }" font-size="300" text-anchor="end">Otevřeno</text>
          </svg>

          <!-- (5596, 21507) -->
          <!-- (15096, 21507) -->
          <!-- (5596, 18407) -->

        </v-card-text>
        <v-divider></v-divider>
        <v-skeleton-loader class="mb-10 mt-6" v-if="loading" type="list-item, list-item"> </v-skeleton-loader>
        <v-card-text v-else>
          <v-row v-if="params.CasDoOtevreni != 0 || params.CasDoZavreni != 0" no-gutters>
            <v-col>
              <v-sheet v-if="params.CasDoOtevreni != 0" class="pa-2 ma-2">
                <v-icon size="28" class="mr-4">{{ mdiWeatherSunsetUp }}</v-icon>
                Otevírání za:
              </v-sheet>
              <v-sheet v-else-if="params.CasDoZavreni != 0" class="pa-2 ma-2">
                <v-icon size="28" class="mr-4">{{ mdiWeatherSunsetDown }}</v-icon>
                Zavírání za:
              </v-sheet>
            </v-col>
            <v-col>
              <v-sheet class="pa-2 ma-2">
                <v-progress-linear v-if="params.CasDoOtevreni != 0" color="orange" v-model="openTimer" height="20">
                  <strong>{{ params.CasDoOtevreni }} min</strong>
                </v-progress-linear>
                <v-progress-linear v-else-if="params.CasDoZavreni != 0" color="primary" v-model="closeTimer" height="20">
                  <strong>{{ params.CasDoZavreni }} min</strong>
                </v-progress-linear>
              </v-sheet>
            </v-col>
          </v-row>
          <v-row v-if="params.AutomatikaOtevreni != 0" no-gutters>
            <v-col cols="8">
              <v-sheet class="pa-2 ma-2">
                <v-icon v-if="params.AutomatikaOtevreni == 1" size="28" class="mr-4">{{ mdiBrightness6 }}</v-icon>
                <v-icon v-else size="28" class="mr-4">{{ mdiSunClock }}</v-icon>
                Otevření v:
              </v-sheet>
            </v-col>
            <v-col>
              <v-sheet class="pa-2 ma-2 text-right">
                {{ autoOpenTime }}
              </v-sheet>
            </v-col>
          </v-row>

          <v-row v-if="params.AutomatikaZavreni != 0" no-gutters>
            <v-col cols="8">
              <v-sheet class="pa-2 ma-2">
                <v-icon v-if="params.AutomatikaZavreni == 1" size="28" class="mr-4">{{ mdiBrightness6 }}</v-icon>
                <v-icon v-else size="28" class="mr-4">{{ mdiSunClock }}</v-icon>
                Zavření v:
              </v-sheet>
            </v-col>
            <v-col>
              <v-sheet class="pa-2 ma-2 text-right">
                {{ autoCloseTime }}
              </v-sheet>
            </v-col>
          </v-row>

          <v-row v-if="params.AutomatikaZavreni == 0 && params.AutomatikaOtevreni == 0" no-gutters>
            <v-col class="text-center">
              <v-icon size="28" color="primary" class="mr-4">{{ mdiCloseCircle }}</v-icon>


              <span class="text-subtitle-1">Automatika není aktivní</span>
            </v-col>

          </v-row>
        </v-card-text>
      </v-card>
    </v-col>

    <v-col cols="12" sm="12" md="6" lg="4" xl="3">
      <v-card>
        <v-card-title class="d-flex justify-space-between align-center">
          <span class="text-medium-emphasis">Vnitřní teplota</span>
          <v-icon color="primary">{{ mdiHomeThermometerOutline }}</v-icon>
        </v-card-title>

        <v-skeleton-loader v-if="loading" type="button"> </v-skeleton-loader>
        <v-card-text v-else class="text-h4 text-left mt-4">
          {{ params.Teplota_C }} °C
        </v-card-text>
        <temperature-chart :array-data="tempChartData" :array-labels="tempChartLabels"
          style="max-height: 200px;"></temperature-chart>

      </v-card>
    </v-col>

    <v-col cols="12" sm="12" md="6" lg="4" xl="3">
      <v-card>
        <v-card-title class="d-flex justify-space-between align-center">
          <span class="text-medium-emphasis">Intenzita světla</span>
          <v-icon color="primary">{{ mdiBrightness6 }}</v-icon>
        </v-card-title>

        <v-skeleton-loader v-if="loading" type="button"> </v-skeleton-loader>
        <v-card-text v-else class="text-h4 text-left mt-4">
          {{ params.Osvetleni_proc }} %
        </v-card-text>
        <light-chart :array-data="lightChartData" :array-labels="lightChartLabels" :treshold-up="params.OsvetleniDen" :treshold-down="params.OsvetleniNoc" style="max-height: 200px;"></light-chart>

      </v-card>
    </v-col>

  </v-row>
</template>
  
<script>
import baseParametersMixin from '@/mixins/baseParametersMixin';
import axios from 'axios';
import TemperatureChart from '@/components/TemperatureChart.vue';
import LightChart from '@/components/LightChart.vue';


const dataFields = {
  CasDoOtevreni: 0,
  CasDoZavreni: 0,
  AktualniCas: "12:20",
  CasVychodu: "7:20",
  CasZapadu: "18:20",
  ZpozdeniOtevreni: 0,
  ZpozdeniZavreni: 0,
  AutomatikaOtevreni: 2,
  AutomatikaZavreni: 2,
  StavDvirka: 0,
  DolniPoloha: 0,
  HorniPoloha: 0,
  Otevreni_proc: 0,
  CasOtevreni: "7:20",
  CasZavreni: "18:20",
  Teplota_C: 0,
  Osvetleni_proc: 0,
  OsvetleniNoc: 0,
  OsvetleniDen: 0,
};

export default {
  mixins: [baseParametersMixin],
  components: {
    TemperatureChart, LightChart
  },
  name: 'HomeView',
  data() {
    return {
      params: dataFields,
      fetchPeriod: 1500,
      tempChartData: null,
      tempChartLabels: null,
      lightChartData: null,
      lightChartLabels: null,
    };
  },
  computed: {
    sunPosition() {
      const sunrise = new Date();
      const sunset = new Date();
      let sun_pos = { x: 0, y: 0 };
      let open_pos = { x: 0, y: 0 };
      let close_pos = { x: 0, y: 0 };

      const sunriseParsed = this.parseTime(this.params.CasVychodu);

      if (isNaN(sunriseParsed.hours) || isNaN(sunriseParsed.minutes)) {
        return { sun: sun_pos, open: open_pos, close: close_pos };
      }
      sunrise.setHours(sunriseParsed.hours, sunriseParsed.minutes, 0, 0);

      const sunsetParsed = this.parseTime(this.params.CasZapadu);
      if (isNaN(sunsetParsed.hours) || isNaN(sunsetParsed.minutes)) {
        return { sun: sun_pos, open: open_pos, close: close_pos };
      }
      sunset.setHours(sunsetParsed.hours, sunsetParsed.minutes, 0, 0);

      const nowTime = this.parseTime(this.params.AktualniCas);
      let day_minutes = nowTime.hours * 60 + nowTime.minutes;
      let sunrise_minutes = (sunrise.getHours() * 60 + sunrise.getMinutes());
      let sunset_minutes = (sunset.getHours() * 60 + sunset.getMinutes());

      sun_pos = this.computeDayPos(day_minutes, sunrise_minutes, sunset_minutes);
      open_pos = this.computeDayPos((sunrise_minutes + this.params.ZpozdeniOtevreni) % 1440, sunrise_minutes, sunset_minutes);
      close_pos = this.computeDayPos((sunset_minutes + this.params.ZpozdeniZavreni) % 1440, sunrise_minutes, sunset_minutes);

      return { sun: sun_pos, open: open_pos, close: close_pos };

    },
    autoOpenTime() {
      const sunrise = new Date();
      const sunriseParsed = this.parseTime(this.params.CasVychodu);
      if (isNaN(sunriseParsed.hours) || isNaN(sunriseParsed.minutes)) {
        return "--:--";
      }
      sunrise.setHours(sunriseParsed.hours, sunriseParsed.minutes, 0, 0);
      sunrise.setMinutes(sunrise.getMinutes() + this.params.ZpozdeniOtevreni);
      const hours = sunrise.getHours().toString().padStart(2, '0');
      const minutes = sunrise.getMinutes().toString().padStart(2, '0');
      return `${hours}:${minutes}`;
    },
    autoCloseTime() {
      const sunset = new Date();
      const sunsetParsed = this.parseTime(this.params.CasZapadu);
      if (isNaN(sunsetParsed.hours) || isNaN(sunsetParsed.minutes)) {
        return "--:--";
      }
      sunset.setHours(sunsetParsed.hours, sunsetParsed.minutes, 0, 0);
      sunset.setMinutes(sunset.getMinutes() + this.params.ZpozdeniZavreni);
      const hours = sunset.getHours().toString().padStart(2, '0');
      const minutes = sunset.getMinutes().toString().padStart(2, '0');
      return `${hours}:${minutes}`;
    },
    openTimer() {
      return this.params.CasDoOtevreni * 100 / this.params.ZpozdeniOtevreni;
    },
    closeTimer() {
      return this.params.CasDoZavreni * 100 / this.params.ZpozdeniZavreni;
    },
    doorPosition() {
      const newYPosStart = 10810 - (this.params.Otevreni_proc * 7000 / 100);
      const newYPosEnd = 19848 - (this.params.Otevreni_proc * 7000 / 100);

      let fill_path = `M 13084,${newYPosStart + 130} C 13084,${newYPosStart + 52} 13023,${newYPosStart + 1} 12954,${newYPosStart + 1} L 12954,${newYPosStart + 1} C 11319,${newYPosStart + 1} 9684,${newYPosStart + 1} 8049,${newYPosStart + 1} L 8049,${newYPosStart + 1} C 7973,${newYPosStart + 1} 7920,${newYPosStart + 59} 7920,${newYPosStart + 130} L 7920,${newYPosStart + 130} 7920,${newYPosEnd} 7920,${newYPosEnd} 7920,${newYPosEnd} 7920,${newYPosEnd} 13084,${newYPosEnd} 13084,${newYPosEnd} 13084,${newYPosEnd} 13084,${newYPosEnd} 13084,${newYPosStart + 130} Z`;

      let stroke_path = `M 13084,${newYPosStart + 130} C 13084,${newYPosStart + 52} 13023,${newYPosStart + 1} 12954,${newYPosStart + 1} L 12954,${newYPosStart + 1} 8049,${newYPosStart + 1} 8049,${newYPosStart + 1} C 7973,${newYPosStart + 1} 7920,${newYPosStart + 59} 7920,${newYPosStart + 130} L 7920,${newYPosStart + 130} 7920,${newYPosEnd} 7920,${newYPosEnd} 7920,${newYPosEnd} 7920,${newYPosEnd} 13084,${newYPosEnd} 13084,${newYPosEnd} 13084,${newYPosEnd} 13084,${newYPosEnd} 13084,${newYPosStart + 130} Z`;

      return { fill: fill_path, stroke: stroke_path };
    }
  },
  methods: {
    generateTimeLabels(dataLength, step) {
      const labels = [];
      const now = new Date();
      for (let i = 0; i < dataLength; i++) {
        const time = new Date(now.getTime() - i * step);
        labels.push(time.toLocaleTimeString('cs-CZ', { hour: '2-digit', minute: '2-digit' }));
      }
      return labels;
    },
    parseTime(timeString) {
      if (timeString.includes('.')) {
        const dateTimeParts = timeString.split(' ');
        const timePart = dateTimeParts[1];
        const [hours, minutes] = timePart.split(':').map(Number);
        return { hours, minutes };
      } else {
        const [hours, minutes] = timeString.split(':').map(Number);
        return { hours, minutes };
      }
    },
    computeDayPos(day_minutes, sunrise_minutes, sunset_minutes) {
      let pos_x = 0;
      let pos_y = 0;
      const isDaytime = day_minutes >= sunrise_minutes && day_minutes <= sunset_minutes;

      let progress, fullDayDuration;
      if (isDaytime) {
        fullDayDuration = sunset_minutes - sunrise_minutes;
        progress = (day_minutes - sunrise_minutes) / fullDayDuration;
        pos_x = (progress * (1140 - 300)) + 300;
      } else {
        fullDayDuration = sunrise_minutes - sunset_minutes;
        if (fullDayDuration < 0) {
          fullDayDuration += 24 * 60;
        }
        let suns_min = day_minutes > sunset_minutes ? day_minutes : (day_minutes + 24 * 60);
        progress = (suns_min - sunset_minutes) / fullDayDuration;
        pos_x = progress * ((1440 + 300) - 1140) + 1140;
      }

      if (pos_x > 1440) {
        pos_x = pos_x - 1440;
      }

      // sunrise_x = 7575 / 300 min
      // sunset_x = 13116 / 1140 min
      // <!-- (5596, 21507) -->
      // <!-- (15096, 21507) -->
      // <!-- (5596, 18407) 

      pos_y = Math.cos(2 * Math.PI * (pos_x) / (60 * 24));
      pos_x = (pos_x / (60 * 24)) * (15096 - 5596) + 5596;
      pos_y = ((21507 - 18407) / 2) * pos_y + ((18407 + 21507) / 2);
      // console.log(pos_x);
      // console.log(pos_y);
      return { x: pos_x, y: pos_y };
    },
    fetchDataCallback(success) {
      // setTimeout(() => {
      //   this.fetchData(false);
      // }, 2000);
    },
    mountCallback() {

      const queryData = {
        Teplota24H: "",
        Svetlo24H: "",
      };

      let queryParams = new URLSearchParams(queryData).toString();
      axios.get(`/api/get_params?${queryParams}`)
        .then(response => {
          if (response.headers['content-type'].includes('application/json')) {

            const temperatureData = response.data.Teplota24H;
            const lightData = response.data.Svetlo24H;
            this.tempChartLabels = this.generateTimeLabels(temperatureData.length, 3 * 60 * 1000);
            this.tempChartData = temperatureData;
            this.lightChartLabels = this.generateTimeLabels(temperatureData.length, 3 * 60 * 1000);
            this.lightChartData = lightData;
          } else {
            throw new Error('Invalid response type');
          }
        })
        .catch(error => {
            console.error('Error loading parameters:', error);
            this.$toast.error('Došlo k chybě při načítání parametrů. Zkuste stránku obnovit.');
        });

    },
    doorCommand(command) {

      const data = {
        Command: command, // 1 for up, 2 for down
      };
      const config = {
        headers: {
          'Content-Type': 'application/json',
        },
      };

      axios.post('/api/set_params', data, config)
        .then(response => {
        })
        .catch(error => {
          this.$toast.error('Při odesílání požadavku došlo k chybě. Zkuste to znovu později.');
        });
    },
  }
}

</script>

<style>

</style>