const expandables = document.querySelectorAll('.expandable');

let isExpanded = false;

expandables.forEach((expandable) =>{
    const header = expandable.querySelector('.header');
    const content = expandable.querySelector('.content');
    const toggle = header.querySelector('.toggle');
    const frame = content.querySelector('.frame')
    toggle.addEventListener('click', () => {
        toggle.querySelector('.toggle-icon').classList.toggle('active');
        if (!isExpanded) {
            content.style.height = content.scrollHeight + 'px';
            if(frame){
                content.style.height = "800px";
                content.style.width = "100%";
                frame.style.height = "670px";
                frame.style.width = "99%";
            }
            isExpanded = true;
        } else {
            content.style.height = '0';
            isExpanded = false;
        }
    });
});

((doc) => {
    //===========================主机程序控制部分===========================
    const hostAddressInput = doc.querySelector('#Host_IP');
    const hostPortInput = doc.querySelector('#Host_Port');
    const shutdownButton = doc.querySelector('#shutdown');
    var ws = null
    let userName = 'Client'
    let passWord = ''
    const init = () => {
        let url = location.search
        if (url.indexOf('?') !== -1) {
            let str = String(url).slice(0)
            let arr =  str.split('&')
            userName = arr[0].split('=')[1]
            passWord = arr[1].split('=')[1]
            console.log('userNamepassWord', userName, passWord)
        }
	    bindEvent()
    }
    function bindEvent() {
        shutdownButton.addEventListener('click',shutdown,false);
    }
    function shutdown(e){
        shutdownButton.style.backgroundColor = '';
        connect();
    }
    connection = false;
    function connect(e) {
        if (!ws){
                ws = new WebSocket('ws:'+hostAddressInput.value+':'+hostPortInput.value);
                ws.addEventListener('open', handleOpen, false);
                ws.addEventListener('close', handleClose, false);
                ws.addEventListener('error', handleError, false);
                ws.addEventListener('message', handleMessage, false);
        }
    }
    function wsSend (msg) {
        if(!connection) return;
        if(!ws) return;
        if (!msg.trim().length) return;
        ws.send(msg);
    }
    function handleOpen (e) {
        connection = true;
        shutdownButton.disabled = true;
        shutdownButton.style.backgroundColor = '#00FF00';
        wsSend('shutdown')
    }
    function handleClose (e) {
        if(connection){
            connection = false;
            shutdownButton.disabled = false;
            shutdownButton.style.backgroundColor = 'red';
        }
        delete ws;
        ws = null;
    }
    function handleError (e) {
        connection = false;
        shutdownButton.disabled = false;
        shutdownButton.style.backgroundColor = '';
        delete ws;
        ws = null;
    }
    function handleMessage (e) {
        var rsp = e.data.split(" ");
    }
    init()
})(document);

((doc) => {
    //===========================传感器部分===========================
    // 获取按钮并禁用
    const Buttons = doc.querySelectorAll('#sensors button');
    Buttons.forEach((button) => {
        button.disabled = true;
    });
    const ConnectionButton = doc.querySelector('#sensors_connect');
    ConnectionButton.disabled = false;
    const InitButton = doc.querySelector('#sensors_init');
    const ShutdownButton = doc.querySelector('#sensors_shutdown');

    const StartButton = doc.querySelector('#sensors_start');
    const StopButton = doc.querySelector('#sensors_stop');

    const presssureSpan = doc.querySelector('#Pressure_Data');
    const temperatureSpan = doc.querySelector('#Temperature_Data');
    const humiditySpan = doc.querySelector('#Humidity_Data');
    const gasFlowSpan = doc.querySelector('#GasFlow_Data');

    const hostAddressInput = doc.querySelector('#Host_IP');
    hostAddressInput.value = '10.155.101.208';
    const hostPortInput = doc.querySelector('#Host_Port');
    hostPortInput.value = '8100';
    
    const storageDirInput = doc.querySelector('#storageDir');
    storageDirInput.value = './output';
    const rootPeriodInput = doc.querySelector('#rootPeriod');
    // rootPeriodInput.value = 60 ;
    const rootPeriodSetButton = doc.querySelector('#rootPeriodSet');

    const temperature_humidityDeviceInput = doc.querySelector('#temperature_humidityDevice');
    temperature_humidityDeviceInput.value = '/dev/ttyUSB0';
    const pressureDeviceInput = doc.querySelector('#pressureDevice');
    pressureDeviceInput.value = '/dev/ttyUSB1';
    const MFCDeviceInput = doc.querySelector('#MFCDevice');
    MFCDeviceInput.value = '/dev/ttyUSB2';

    const targetpressureInput = doc.querySelector('#targetpressure');
    const targetpressureSetButton = doc.querySelector('#targetpressureSet');
    const targetgasflowInput = doc.querySelector('#targetgasflow');
    const targetgasflowSetButton = doc.querySelector('#targetgasflowSet');
    const pressureLowerLimitInput = doc.querySelector('#pressureLowerLimit');
    // pressureLowerLimitInput.value = '2720';
    const pressureLowerLimitSetButton = doc.querySelector('#pressureLowerLimitSet');

    const pressureUpperLimitInput = doc.querySelector('#pressureUpperLimit');
    // pressureUpperLimitInput.value = '2735';
    const pressureUpperLimitSetButton = doc.querySelector('#pressureUpperLimitSet');

    const pressurePlotMinInput = doc.querySelector('#pressurePlotMin');
    // pressurePlotMinInput.value = '2720';
    const pressurePlotMinSetButton = doc.querySelector('#pressurePlotMinSet');

    const pressurePlotMaxInput = doc.querySelector('#pressurePlotMax');
    // pressurePlotMaxInput.value = '2735';
    const pressurePlotMaxSetButton = doc.querySelector('#pressurePlotMaxSet');

    const pressureDataLengthInput = doc.querySelector('#pressureDataLength');
    pressureDataLengthInput.value = '100';
    const pressureDataLengthSetButton = doc.querySelector('#pressureDataLengthSet');

    const mailAddressInput = doc.querySelector('#DutyEMail');
    const mailAddressAddButton = doc.querySelector('#DutyEMailAdd');
    const mailAddressRemoveButton = doc.querySelector('#DutyEMailRemove');
    const mailListSpan = doc.querySelector('#MailList');

    const phoneNumberInput = doc.querySelector('#DutyPhone');
    const phoneNumberAddButton = doc.querySelector('#DutyPhoneAdd');
    const phoneNumberRemoveButton = doc.querySelector('#DutyPhoneRemove');
    const phoneListSpan = doc.querySelector('#PhoneList');


    const ctx = document.getElementById('pressure_MFCflowChart').getContext('2d');
    const pressure_MFCflowChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: Array.from({ length: pressureDataLengthInput.value }, (_, index) => index.toString()),
            datasets: [{
                label: 'Pressure',
                data: [],
                fill: false,
                borderColor: '#2196F3',
                tension: 0.1,
                yAxisID: 'y1',
            }, {
                label: 'Gas Flow',
                data: [],
                fill: false,
                borderColor: '#F44336',
                tension: 0.1,
                yAxisID: 'y2',
            }]
        },
        options: {
            animation: false,
            scales: {
                y1: {
                    type: 'linear',
                    position: 'left',
                    title: {
                        display: true,
                        text: 'Pressure (Pa)'
                    }
                },
                y2: {
                    type: 'linear',
                    position: 'right',
                    title: {
                        display: true,
                        text: 'Gas Flow (mL/min)'
                    }
                },
                x: {
                    title: {
                        display: true,
                        text: 'Time'
                    }
                }
            }
        }
    });

    const ctx2 = document.getElementById('temperature_humidityChart').getContext('2d');
    const temperature_humidityChart = new Chart(ctx2, {
        type: 'line',
        data: {
            labels: Array.from({ length: pressureDataLengthInput.value }, (_, index) => index.toString()),
            datasets: [{
                label: 'Temperature',
                data: [],
                fill: false,
                borderColor: '#1F2A38',
                tension: 0.1,
                yAxisID: 'y1',
            }, {
                label: 'Humidity',
                data: [],
                fill: false,
                borderColor: '#FFC107',
                tension: 0.1,
                yAxisID: 'y2',
            }]
        },
        options: {
            animation: false,
            scales: {
                y1: {
                    type: 'linear',
                    position: 'left',
                    title: {
                        display: true,
                        text: 'Temperature (°C)'
                    }
                },
                y2: {
                    type: 'linear',
                    position: 'right',
                    title: {
                        display: true,
                        text: 'Humidity (%RH)'
                    }
                },
                x: {
                    title: {
                        display: true,
                        text: 'Time'
                    }
                }
            }
        }
    });

    var ws = null
    let userName = 'Client'
    let passWord = ''
    const init = () => {
        let url = location.search
        if (url.indexOf('?') !== -1) {
            let str = String(url).slice(0)
            let arr =  str.split('&')
            userName = arr[0].split('=')[1]
            passWord = arr[1].split('=')[1]
            console.log('userNamepassWord', userName, passWord)
        }
	    bindEvent()
    }
    function bindEvent() {
        ConnectionButton.addEventListener('click', connect, false);
        InitButton.addEventListener('click', Init, false);
        ShutdownButton.addEventListener('click',Shutdown, false);
	    StartButton.addEventListener('click',Start,false);
	    StopButton.addEventListener('click',Stop,false);        
        rootPeriodSetButton.addEventListener('click',rootPeriodSet, false);
        targetpressureSetButton.addEventListener('click', targetpressureSet, false);
        targetgasflowSetButton.addEventListener('click', targetgasflowSet, false);
        pressureLowerLimitSetButton.addEventListener('click', pressureLowerLimitSet, false);
        pressureUpperLimitSetButton.addEventListener('click', pressureUpperLimitSet, false);
        pressurePlotMinSetButton.addEventListener('click', pressurePlotMinSet, false);
        pressurePlotMaxSetButton.addEventListener('click', pressurePlotMaxSet, false);
        pressureDataLengthSetButton.addEventListener('click', pressureDataLengthSet, false);
        mailAddressAddButton.addEventListener('click', mailAddressAdd, false);
        mailAddressRemoveButton.addEventListener('click', mailAddressRemove, false);
        phoneNumberAddButton.addEventListener('click', phoneNumberAdd, false);
        phoneNumberRemoveButton.addEventListener('click', phoneNumberRemove, false);
    }
    function rootPeriodSet(){
        ConnectionButton.style.backgroundColor = '';
        const command = `rootPeriodSet ${rootPeriodInput.value}`;
        wsSend(command)
    }
    function playAlarm() {
	    var alarm = document.getElementById('alarmSound');
	    alarm.play();
    }
    function mailAddressAdd(e) {
        const command = `mailAddressAdd ${mailAddressInput.value}`;
        wsSend(command)
    }

    function mailAddressRemove(e) {
        const command = `mailAddressRemove ${mailAddressInput.value}`;
        wsSend(command)
    }

    function phoneNumberAdd(e) {
        const command = `phoneNumberAdd ${phoneNumberInput.value}`;
        wsSend(command)
    }

    function phoneNumberRemove(e) {
        const command = `phoneNumberRemove ${phoneNumberInput.value}`;
        wsSend(command)
    }

    function targetpressureSet(e) {
        const command = `targetpressureSet ${targetpressureInput.value}`;
        wsSend(command)
    }

    function targetgasflowSet(e) {
        const command = `targetgasflowSet ${targetgasflowInput.value}`;
        wsSend(command)
    }

    function pressureLowerLimitSet(e) {
        const command = `pressureLowerLimitSet ${pressureLowerLimitInput.value}`;
        wsSend(command)
    }
    function pressureUpperLimitSet(e) {
        const command = `pressureUpperLimitSet ${pressureUpperLimitInput.value}`;
        wsSend(command)
    }
    function pressurePlotMinSet(e) {
        pressure_MFCflowChart.options.scales.y.min = parseFloat(pressurePlotMinInput.value);;
        pressure_MFCflowChart.update();
    }
    function pressurePlotMaxSet(e) {
        pressure_MFCflowChart.options.scales.y.max = parseFloat(pressurePlotMaxInput.value);
        pressure_MFCflowChart.update();
    }
    function pressureDataLengthSet(e) {
        pressure_MFCflowChart.data.labels = Array.from({ length: pressureDataLengthInput.value }, (_, index) => index.toString());
        pressure_MFCflowChart.update();
    }

    function Stop(e) {
        wsSend('stopSensors');
        StopButton.disabled = true;
        StartButton.disabled = false;
    }
    function Start(e) {
        wsSend('startSensors');
        StartButton.disabled = true;
        StopButton.disabled = false;
    }
    function Init(e){
        const command = `initSensors ${storageDirInput.value} ${temperature_humidityDeviceInput.value} ${pressureDeviceInput.value} ${MFCDeviceInput.value}`;
        wsSend(command)
    }
    function Shutdown(e){
        wsSend('shutdownSensors');
    }
    connection = false;
    function connect(e) {
        if (!ws){
            ws = new WebSocket('ws:'+hostAddressInput.value+':'+hostPortInput.value);
            ws.addEventListener('open', handleOpen, false);
            ws.addEventListener('close', handleClose, false);
            ws.addEventListener('error', handleError, false);
            ws.addEventListener('message', handleMessage, false);
        }
    }
    function wsSend (msg) {
        if(!connection) return;
        if(!ws) return;
        if (!msg.trim().length) return;
        ws.send(msg);
    }
    function handleOpen (e) {
        connection = true;
        Buttons.forEach((button) => {
            button.disabled = false;
        });
	    ConnectionButton.style.backgroundColor = '#00FF00';
        ConnectionButton.disabled = true;
        InitButton.style.backgroundColor = 'yellow';
        wsSend('register')
    }
    function handleClose (e) {
        connection = false;
        Buttons.forEach((button) => {
            button.disabled = true;
            button.style.backgroundColor='';
        });
        ConnectionButton.disabled = false;
        delete ws;
        ws = null;
    }
    function handleError (e) {
        connection = false;
        Buttons.forEach((button) => {
            button.disabled = true;
            button.style.backgroundColor='';
        });
        ConnectionButton.disabled = false;
        delete ws;
        ws = null;
    }
    function handleMessage (e) {
        var rsp = e.data.split(" ");
        if(rsp[0]=="rootPeriodSet"){
            rootPeriodSetButton.style.backgroundColor = '#00FF00';
        }
        if(rsp[0]=="pressure"){
            presssureSpan.innerText = parseFloat(rsp[1]).toFixed(2);
            const newData = parseFloat(rsp[1]).toFixed(2);
            pressure_MFCflowChart.data.datasets[0].data.push(newData);
            if (pressure_MFCflowChart.data.datasets[0].data.length > pressureDataLengthInput.value) {
                pressure_MFCflowChart.data.datasets[0].data.shift(); // 移除最旧的数据点
            }
            // 更新图表
            pressure_MFCflowChart.update();
	        //播放报警声
	        if(newData<pressureLowerLimitInput.value&&newData>0){
		        playAlarm();
                alert("Pressure is too low!");
	        }
	        if(newData>pressureUpperLimitInput.value&&newData>0){
		        playAlarm();
                alert("Pressure is too high!");
	        }
        }
        if(rsp[0]=="gasFlow"){
            gasFlowSpan.innerText = parseFloat(rsp[1]).toFixed(2);
            const newData = parseFloat(rsp[1]).toFixed(2);
            pressure_MFCflowChart.data.datasets[1].data.push(newData);
            if (pressure_MFCflowChart.data.datasets[1].data.length > pressureDataLengthInput.value) {
                pressure_MFCflowChart.data.datasets[1].data.shift(); // 移除最旧的数据点
            }
            // 更新图表
            pressure_MFCflowChart.update();
        }
        if(rsp[0]=="temperature"){
            temperatureSpan.innerText = parseFloat(rsp[1]).toFixed(2);
            const newData = parseFloat(rsp[1]).toFixed(2);
            temperature_humidityChart.data.datasets[0].data.push(newData);
            if (temperature_humidityChart.data.datasets[0].data.length > pressureDataLengthInput.value) {
                temperature_humidityChart.data.datasets[0].data.shift(); // 移除最旧的数据点
            }
            // 更新图表
            temperature_humidityChart.update();
        }
        if(rsp[0]=="humidity"){
            humiditySpan.innerText = parseFloat(rsp[1]).toFixed(2);
            const newData = parseFloat(rsp[1]).toFixed(2);
            temperature_humidityChart.data.datasets[1].data.push(newData);
            if (temperature_humidityChart.data.datasets[1].data.length > pressureDataLengthInput.value) {
                temperature_humidityChart.data.datasets[1].data.shift(); // 移除最旧的数据点
            }
            // 更新图表
            temperature_humidityChart.update();
        }
        if(rsp[0]=="sensorsState"){
            if(rsp[1]=="0"){
                InitButton.disabled = true;
                ShutdownButton.disabled = true;
                StartButton.disabled = false;
                StopButton.disabled = true;
                InitButton.style.backgroundColor = '#00FF00';
                ShutdownButton.style.backgroundColor = '';
                StartButton.style.backgroundColor = 'yellow';
                StopButton.style.backgroundColor = '';
            }else if(rsp[1]=="2"){
                InitButton.disabled = true;
                ShutdownButton.disabled = true;
                StartButton.disabled = true;
                StopButton.disabled = false;
                InitButton.style.backgroundColor = '#00FF00';
                ShutdownButton.style.backgroundColor = '';
                StartButton.style.backgroundColor = '#00FF00';
                StopButton.style.backgroundColor = '';
            }else if(rsp[1]=="4"){
                InitButton.disabled = true;
                ShutdownButton.disabled = false;
                StartButton.disabled = true;
                StopButton.disabled = true;
                InitButton.style.backgroundColor = '';
                ShutdownButton.style.backgroundColor = '';
                StartButton.style.backgroundColor = '';
                StopButton.style.backgroundColor = 'red';
            }
        }
        if(rsp[0]=="mailAddress"){
            mailListSpan.innerText = rsp;
        }
        if(rsp[0]=="phoneNumber"){
            phoneListSpan.innerText = rsp;
        }
    }
    function dump(){
        wsSend('getPressure');
        wsSend('getGasFlow');
        wsSend('getTemperature');
        wsSend('getHumidity');
        wsSend('getSensorsState')
    }
    setInterval(dump, 1000);
    init() 
})(document);
