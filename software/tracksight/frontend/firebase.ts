// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getDatabase } from "firebase/database";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional

// this is matt's api key, you can get your own by going to the firebase console if needed (it is free)
const firebaseConfig = {
  apiKey: "AIzaSyDkLiwQaygM79Q1OfC5YHZS0lgp_mix4jo",
  authDomain: "telemetry-5bc72.firebaseapp.com",
  projectId: "telemetry-5bc72",
  storageBucket: "telemetry-5bc72.appspot.com",
  messagingSenderId: "201527675645",
  appId: "1:201527675645:web:5b99c05cc6357ac6964c94",
  measurementId: "G-1RSQKEB2SM"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

export default database;