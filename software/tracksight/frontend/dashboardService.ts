import FLASK_URL from "./src/app/page"

// Function for getting data
const getRealtimeData = async (path: string) => {
    try {
        const response = await fetch(`${FLASK_URL}/get-data?path=${path}`);
        const data = await response.json();
        if (data) {
            return data;
        } else {
            console.log("No data available");
            return null;
        }
    } catch (error) {
        console.error(error);
    }
};

// Function for saving data
const saveDashboardData = async (path: string, data: any) => {
    try {
        const response = await fetch(`${FLASK_URL}/save-data`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ path, data }),
        });
        if (response.ok) {
            return true;
        } else {
            throw new Error('Failed to save data');
        }
    } catch (error) {
        console.error("Error saving dashboard data: ", error);
        return false;
    }
};

// Function for deleting data
const deleteDashboardData = async (path: string) => {
    try {
        const response = await fetch(`${FLASK_URL}/delete-data`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ path }),
        });
        if (response.ok) {
            return true;
        } else {
            throw new Error('Failed to delete data');
        }
    } catch (error) {
        console.error("Error deleting dashboard data: ", error);
        return false;
    }
};

export { getRealtimeData, saveDashboardData, deleteDashboardData };
