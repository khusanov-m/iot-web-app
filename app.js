import { initializeApp } from "https://www.gstatic.com/firebasejs/10.6.0/firebase-app.js";
import {
  getDatabase,
  onValue,
  ref,
  set,
} from "https://www.gstatic.com/firebasejs/10.6.0/firebase-database.js";

// DOM Elements
const status = document.getElementById("status");
const moisture = document.getElementById("moisture");
const waterlvl = document.getElementById("waterlvl");
const temperature = document.getElementById("temperature");
const humidity = document.getElementById("humidity");

// Firebase Config
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

// Firebase record edit value by key setup
function editValue(fieldName, value) {
  set(ref(db, fieldName), value);
}

// Read and setup data correspondingly
const refDB = ref(db);
onValue(refDB, (snapshot) => {
  const data = snapshot.val();
  status.checked = data.LedStatus;
  moisture.textContent = data.MoistureLevel;
  waterlvl.textContent = data.WaterLevel;
  temperature.textContent = data.Temperature;
  humidity.textContent = data.Humidity;

  console.log(data.WateringCounts);
  const ws = Object.keys(data.WateringCounts).map((key) =>
    JSON.parse(data.WateringCounts[key].replace(/'/g, '"'))
  );
  console.log(ws);
  prepareGraph(ws);
});

// Setup event listeners
status.addEventListener("change", (e) => {
  editValue("LedStatus", e.target.checked ? 1 : 0);
});

// Chart setup
const ctx = document.getElementById("myChart");

function prepareGraph(data) {
  let total = 0;
  const sets = [];
  data.forEach((d, i) => {
    console.log(d);
    total++;
    if (data[i + 1]?.iteration > d?.iteration) {
      sets.push(total);
      total = 0;
    } else if (i === data.length - 1) {
      sets.push(total);
    }
  });

  // Limit to last 7 iterations (days)
  if (sets.length > 7) {
    sets.splice(0, sets.length - 7);
  }
  updateChart(sets);
}

const chartID = new Chart(ctx, {
  type: "bar",
  data: {
    labels: [],
    datasets: [
      {
        label: "# of Waterings Count",
        data: [],
        borderWidth: 1,
      },
    ],
  },
  options: {
    scales: {
      y: {
        beginAtZero: true,
      },
    },
  },
});

function updateChart(stats) {
  chartID.data.labels = stats.map((_, i) => i + 1);
  chartID.data.datasets[0].data = stats;
  chartID.update();
}
