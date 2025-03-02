import { ref } from 'vue';

export const accessLevel = ref(localStorage.getItem('accessLevel') || '0');

export function updateAccessLevel(newLevel) {
    localStorage.setItem('accessLevel', newLevel);
    accessLevel.value = newLevel;
}
