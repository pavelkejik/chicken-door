import Toast, { POSITION, useToast } from 'vue-toastification';

export const ToastPlugin = {
  install(app) {
    app.use(Toast, {
      position: POSITION.TOP_RIGHT,
    });

    app.config.globalProperties.$toast = useToast();
  },
};
