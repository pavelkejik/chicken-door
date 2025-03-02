export default {
    data() {
      return {
        isMobile: window.innerWidth <= 600
      };
    },
    created() {
      this.handleResize();
      window.addEventListener('resize', this.handleResize);
    },
    mounted() {
        this.adjustLayout(this.isMobile);
      },
    beforeDestroy() {
      window.removeEventListener('resize', this.handleResize);
    },
    methods: {
      handleResize() {
        const isMobile = window.innerWidth <= 600;
        if (this.isMobile !== isMobile) {
          this.isMobile = isMobile;
          this.adjustLayout(isMobile);
        }
      },
      adjustLayout(isMobile) {
        if (!isMobile) {
          this.drawer = true; // Ensure the sidebar is visible in desktop
        } else {
          this.drawer = false;
          this.rail = false;
        }
      }
    }
  };
  