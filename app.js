import { initializeApp } from "https://www.gstatic.com/firebasejs/10.6.0/firebase-app.js";
import {
  getDatabase,
  onValue,
  ref,
  set,
} from "https://www.gstatic.com/firebasejs/10.6.0/firebase-database.js";
const status = document.getElementById("status");
const moisture = document.getElementById("moisture");
const led = document.getElementById("led");

const firebaseConfig = {
  apiKey: "AIzaSyB1GG3TGB-PDC_zu3ETt8g7Ua5gbOMFU0g",
  authDomain: "iot-00011883.firebaseapp.com",
  databaseURL: "https://iot-00011883-default-rtdb.firebaseio.com",
  projectId: "iot-00011883",
  storageBucket: "iot-00011883.appspot.com",
  messagingSenderId: "818854426856",
  appId: "1:818854426856:web:ccb35b017692b5631ef15d",
  measurementId: "G-9XYKF7QSF0",
};
// Initialize Firebase
const app = initializeApp(firebaseConfig);
const db = getDatabase();

// Edit value setup
function editValue(fieldName, value) {
  set(ref(db, fieldName), value);
}

// Read and set data
const refDB = ref(db);
onValue(refDB, (snapshot) => {
  const data = snapshot.val();
  led.textContent = data.LedStatus === 0 ? "OFF" : "ON";
  status.checked = data.LedStatus;
  moisture.textContent = data.MoistureLevel;
});

status.addEventListener("change", (e) => {
  editValue("LedStatus", e.target.checked ? 1 : 0);
});
