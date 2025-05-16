export const deviceEnums = {
    1: { // Feeder
      commandFlags: [
        { value: 0, text: 'Stop' },
        { value: 1, text: 'Otevřít' },
        { value: 2, text: 'Zavřít' },
      ],
      stateFlags: [
        { value: 0, text: 'Neznámá Poloha' },
        { value: 1, text: 'Otevírání' },
        { value: 2, text: 'Otevřeno' },
        { value: 3, text: 'Uvolnění Otevírání' },
        { value: 4, text: 'Ověření Otevření' },
        { value: 5, text: 'Zavírání' },
        { value: 6, text: 'Zavřeno' },
        { value: 7, text: 'Uvolnění Zavírání' },
        { value: 8, text: 'Ověření Zavření' },
        { value: 9, text: 'Stop Zavírání' },
        { value: 10, text: 'Stop Otevírání' },
        { value: 11, text: 'Chyba Krmitka' },
      ],
    },
    2: { // Camera
        commandFlags: [
            { value: 0, text: 'Stop' },
            { value: 1, text: 'Otevřít' },
            { value: 2, text: 'Zavřít' },
          ],
          stateFlags: [
            { value: 0, text: 'Automaticky' },
            { value: 1, text: 'Vždy' },
            { value: 2, text: 'Nikdy' },
          ],
    },
        3: { // EggCamera
        commandFlags: [
            { value: 0, text: 'Stop' },
            { value: 1, text: 'Otevřít' },
            { value: 2, text: 'Zavřít' },
          ],
          stateFlags: [
            { value: 0, text: 'Automaticky' },
            { value: 1, text: 'Vždy' },
            { value: 2, text: 'Nikdy' },
          ],
    },
  };