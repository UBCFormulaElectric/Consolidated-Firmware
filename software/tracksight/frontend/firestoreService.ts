import database from './firebase'; 
import { ref, get, set } from "firebase/database";


// function for getting  data from firebase
const getRealtimeData = async (path: string) => {
    try {
        const dataRef = ref(database, path);
        const snapshot = await get(dataRef);
        if (snapshot.exists()) {
            return snapshot.val();
        } else {
            console.log("No data available");
            return null;
        }
    } catch (error) {
        console.error(error);
    }
};

// function for saving data to firebase
const saveDashboardData = async (path: string, data: any) => {
    try {
        const dataRef = ref(database, path);
        await set(dataRef, data);
        return true; 
    } catch (error) {
        console.error("Error saving dashboard data: ", error);
        return false; 
    }
};


// function for deleting data from firebase
const deleteDashboardData = async (path: string) => {
    try {
        const dataRef = ref(database, path);
        await set(dataRef, null); 
        return true;
    } catch (error) {
        console.error("Error deleting dashboard data: ", error);
        return false;
    }
};

export { getRealtimeData, saveDashboardData, deleteDashboardData };
