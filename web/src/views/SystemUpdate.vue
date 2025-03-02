<template>
    <v-card elevation="4">
        <div class="card-title-container ma-4">
            <div class="title-line"></div>
            <v-card-title class="headline text-center">Aktualizace</v-card-title>
            <div class="title-line"></div>
        </div>

        <v-card-text>

            <div class="text-caption text-medium-emphasis ml-2 mb-4">
                <p class="text-yellow-darken-3">Nahrajte níže nový firmware (firmware.bin) nebo souborový systém
                    (littlefs.bin). Pro uplatnění změn firmware je potřeba zařízení restartovat.</p>
            </div>
            <v-file-input show-size v-model="selectedFile" label="File input" variant="outlined" accept=".bin"
                ></v-file-input>


            <v-card-actions>

                <v-btn class="ml-8" v-if="selectedFile" @click="uploadFile" variant="outlined" color="primary">

                    <v-icon left class="mr-2">{{ mdiFileUpload }}</v-icon>

                    Nahrát</v-btn>
            </v-card-actions>

        </v-card-text>

    </v-card>

    <v-dialog v-model="dialog" persistent max-width="600px">
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
</template>
  
<script>
import baseParametersMixin from '@/mixins/baseParametersMixin';
import axios from 'axios';

const dataFields = {
};


export default {
    mixins: [baseParametersMixin],
    name: 'SystemUpdate',
    data: () => ({
        params: dataFields,
        dialog: false,
        uploadProgress: 0,
        selectedFile: null,
    }),
    methods:
    {
        uploadFile() {
            if (!this.selectedFile) {
                return;
            }

            let uploadUrl;
            if (this.selectedFile[0].name === "firmware.bin") {
                uploadUrl = '/api/fw_update';
            } else if (this.selectedFile[0].name === "littlefs.bin") {
                uploadUrl = '/api/fs_update';
            } else {
                this.$toast.error('Nevalidní soubor. Prosím nahrajte soubor "firmware.bin" nebo "littlefs.bin".');
                return;
            }

            this.dialog = true;
            const formData = new FormData();
            formData.append('file', this.selectedFile[0]);

            axios.post(uploadUrl, formData, {
                headers: {
                    'Content-Type': 'multipart/form-data',
                },
                onUploadProgress: (progressEvent) => {
                    const progress = parseInt(Math.round((progressEvent.loaded * 100) / progressEvent.total));
                    this.uploadProgress = progress;
                },
            })
                .then(response => {
                    this.dialog = false; 
                    this.$toast.success('Soubor byl úspěšně nahrán.');
                    this.uploadProgress = 0;
                })
                .catch(error => {
                    this.dialog = false;
                    console.error('Upload failed:', error);
                    this.$toast.error('Při nahrávání souboru došlo k chybě. Prosím, zkuste to znovu později.');
                    this.uploadProgress = 0;
                });
        },
    },
};
</script>