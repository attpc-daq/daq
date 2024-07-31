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
	//wsSend('register')
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
        //console.log(rsp);
    }
    init()
})(document);

((doc) => {
    //===========================FEE部分===========================
    // 获取电子学部分的按钮并禁用
    const feeButtons = doc.querySelectorAll('#FEE button');
    feeButtons.forEach((button) => {
        button.disabled = true;
    });
    // 获取电子学部分的按钮
    const feeConnectionButton = doc.querySelector('#FEE_connect');
    feeConnectionButton.disabled = false;
    const feeInitButton = doc.querySelector('#FEE_init');
    const feeShutdownButton = doc.querySelector('#FEE_shutdown');

    const feeStartButton = doc.querySelector('#FEE_start');
    const feeStopButton = doc.querySelector('#FEE_stop');

    const feeDataStartButton = doc.querySelector('#FEE_data_start');
    const feeDataStopButton = doc.querySelector('#FEE_data_stop');
    const feeSetupButton = doc.querySelector('#FEE_setup');
    const feeselfhitButton = doc.querySelector('#FEE_selftrigger');
    const decoderStartButton = doc.querySelector('#FEE_start_decoder');
    const decoderStopButton = doc.querySelector('#FEE_stop_decoder');

    const feesetupthresholdButton = doc.querySelector('#setupthreshold');
    const feeThresholdFilePathInput = doc.querySelector('#thresholdFilePath');
    feeThresholdFilePathInput.value = './output/thresholdes.json'
    const feeSpanThresholdProcess = doc.querySelector('#thresholdprocess');
    // const feeslopeButton = doc.querySelector('#slope');
    // const feenchannelButton = doc.querySelector('#nchannel');

    // 设置主机地址的默认值
    const hostAddressInput = doc.querySelector('#Host_IP');
    hostAddressInput.value = '10.155.101.208';
    // 设置端口的默认值
    const hostPortInput = doc.querySelector('#Host_Port');
    hostPortInput.value = '8000';
    // 设置电子学部分的SiTCP地址和端口

    const siTcpAddress_1_Input = doc.querySelector('#FEE1_addr');
    siTcpAddress_1_Input.value = '192.168.10.16';
    // siTcpAddress_1_Input.value = '0.0.0.0';
    const siTcpport_1_Input = doc.querySelector('#FEE1_port');
    siTcpport_1_Input.value = '4660';

    const bufferFilePathInput1 = doc.querySelector('#FEE1_bufferPath');
    bufferFilePathInput1.value = './output/buffers1' ;

    const siTcpAddress_2_Input = doc.querySelector('#FEE2_addr');
    siTcpAddress_2_Input.value = '192.168.10.15';
    const siTcpport_2_Input = doc.querySelector('#FEE2_port');
    siTcpport_2_Input.value = '4660';

    const bufferFilePathInput2 = doc.querySelector('#FEE2_bufferPath');
    bufferFilePathInput2.value = './output/buffers2' ;

    const bufferFileSizeInput = doc.querySelector('#FEE_bufferSize');
    bufferFileSizeInput.value = '100000000';

    const nTaskSpan1 = doc.querySelector('#DAQ1_nTask');
    const dataRateSpan1 = doc.querySelector('#DAQ1_dataRate');
    const nTaskSpan2 = doc.querySelector('#DAQ2_nTask');
    const dataRateSpan2 = doc.querySelector('#DAQ2_dataRate');

    const nQueueSpan1 = doc.querySelector('#DAQ1_nQueue');
    const nQueueSpan2 = doc.querySelector('#DAQ2_nQueue');

    // 设置触发斜率的默认值
    const slopthresholdInput = doc.querySelector('#slope');
    slopthresholdInput.value = '0x0020';
    // 设置符合通道的默认值
    const nchannelInput = doc.querySelector('#nchannel');
    nchannelInput.value = '0x0004';

    // 获取T0_check复选框和后续的输入字段
    const T0Check = document.getElementById("T0_check");
    const T0Type = document.getElementById("T0_type");
    const selfT0Frequency = document.querySelector('input[placeholder="self-T0频率，比如50，单位Hz"]');
    selfT0Frequency.value = '50';
    const startTime = document.querySelector('input[placeholder="开始响应hit的时间，单位ns"]');
    startTime.value = '0';
    const stopTime = document.querySelector('input[placeholder="停止响应hit的时间，单位ns"]');
    stopTime.value = '10000000';
    const T0_Button = document.getElementById("T0_button");
    
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
        feeConnectionButton.addEventListener('click', connect, false);
        feeInitButton.addEventListener('click', feeInit, false);
        feeShutdownButton.addEventListener('click',feeShutdown, false);
	    feeStartButton.addEventListener('click',feeStart,false);
	    feeStopButton.addEventListener('click',feeStop,false);
	    feeDataStartButton.addEventListener('click',feeDataStart,false);
	    feeDataStopButton.addEventListener('click',feeDataStop,false);
	    feeSetupButton.addEventListener('click',feeSetup,false);
        feeselfhitButton.addEventListener('click',feeselfhit,false);
        decoderStartButton.addEventListener('click',decoderStart,false);
        decoderStopButton.addEventListener('click',decoderStop,false);
        bufferFileSizeInput.addEventListener('keydown',setBufferSize,false);
	    feesetupthresholdButton.addEventListener('click',feesetupthreshold,false);
	    // feeslopeButton.addEventListener('click',feeslope,false);
        // feenchannelButton.addEventListener('click',feenchannel,false);
        slopthresholdInput.addEventListener('keydown',feeslope,false);
        nchannelInput.addEventListener('keydown',feenchannel,false);
        T0Check.addEventListener('change', T0CheckChange, false);
        T0_Button.addEventListener("click",setT0,false);
        bufferFilePathInput1.addEventListener('keydown',setBufferFilePath1,false);
        bufferFilePathInput2.addEventListener('keydown',setBufferFilePath2,false);
        
    }
    function setBufferFilePath1(e){
        if(e.key==='Enter'){
            const command = `setSiTCP1BufferFilePath ${bufferFilePathInput1.value}`;
            wsSend(command);
        }
    }
    function setBufferFilePath2(e){
        if(e.key==='Enter'){
            const command = `setSiTCP2BufferFilePath ${bufferFilePathInput2.value}`;
            wsSend(command);
        }
    }
    function setBufferSize(e){
        if(e.key==='Enter'){
            // FEE_setBufferSizeButton.disabled = true;
            bufferFileSizeInput.disabled = true;
            const command = `setBufferFileSize ${bufferFileSizeInput.value}`;
            wsSend(command)
        }
    }
    async function setT0(e){
        T0_Button.disabled = true;
        if(T0Check.checked){
            const command = `set_T0 ${T0Type.value} ${selfT0Frequency.value} ${startTime.value} ${stopTime.value}`;
            wsSend(command);
        }
    }
    async function T0CheckChange(e) {
        const isChecked = T0Check.checked;
        T0Type.disabled = !isChecked;
        selfT0Frequency.disabled = !isChecked;
        startTime.disabled = !isChecked;
        stopTime.disabled = !isChecked;
        T0_Button.disabled = !isChecked;
    }
    async function feenchannel(e) {
        if(e.key==='Enter'){
            const value = nchannelInput.value;
            const cmd = '02102831'+'4'+value[5]+'5'+value[4]+'6'+value[3]+'7'+value[2]+'83';
            const command = `setfeenchannel ${cmd}`;
            wsSend(command);
            nchannelInput.backgroundColor = 'yellow';
            nchannelInput.disabled = true;
            
        }
    }
    async function feeslope(e) {
        if(e.key==='Enter'){
            const value = slopthresholdInput.value;
            const cmd = '00112831'+'4'+value[5]+'5'+value[4]+'6'+value[3]+'7'+value[2]+'83';
            const command = `setfeeslope ${cmd}`;
            wsSend(command);
            slopthresholdInput.backgroundColor = 'yellow';
            slopthresholdInput.disabled = true;
            
        }
    }
    function feesetupthreshold(e) {
        const value = feeThresholdFilePathInput.value;
        wsSend('setupthreshold '+ `${value}`);
        feesetupthresholdButton.disabled = true;
        feesetupthresholdButton.style.backgroundColor = '';
    }
    
    function feeselfhit(e) {
        wsSend('selftrigger');
        feeselfhitButton.disabled = true;
        feeselfhitButton.style.backgroundColor = "";
    }
    function feeSetup(e) {
        feesetupthreshold(e);
        feeslope(e);
        feenchannel(e);
        // feehit_mode(e);//TODO:?
    }
    function feeStop(e) {
        wsSend('stopSiTCP');
        feeStopButton.disabled = true;
        feeStartButton.disabled = false;
    }
    function feeStart(e) {
        wsSend('startSiTCP');
        feeStartButton.disabled = true;
        feeStopButton.disabled = false;
    }
    function feeDataStop(e) {
        wsSend('stopSiTCPData');
        feeDataStopButton.disabled = true;
        feeDataStartButton.disabled = false;
    }
    function feeDataStart(e) {
        wsSend('startSiTCPData');
        feeDataStartButton.disabled = true;
        feeDataStopButton.disabled = false;
    }
    function decoderStart(){
        wsSend('startSiTCPDecoder');
    }
    function decoderStop(){
        wsSend('stopSiTCPDecoder');
    }
    function feeInit(e){
        const command = `initSitcp ${siTcpAddress_1_Input.value} ${siTcpport_1_Input.value} ${bufferFilePathInput1.value} ${siTcpAddress_2_Input.value} ${siTcpport_2_Input.value} ${bufferFilePathInput2.value} ${bufferFileSizeInput.value}`;
        wsSend(command)
    }
    function feeShutdown(e){
        wsSend('shutdownSiTCP');
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
        feeButtons.forEach((button) => {
            button.disabled = false;
        });
	feeConnectionButton.style.backgroundColor = '#00FF00';
        feeConnectionButton.disabled = true;
        feeInitButton.style.backgroundColor = 'yellow';
        wsSend('register')
    }
    function handleClose (e) {
        connection = false;
        feeButtons.forEach((button) => {
            button.disabled = true;
            button.style.backgroundColor='';
        });
        feeConnectionButton.disabled = false;
        delete ws;
        ws = null;
        dataRateSpan1.innerText = '-';
        dataRateSpan2.innerText = '-';
        nTaskSpan1.innerText = '-';
        nTaskSpan2.innerText = '-';
        nQueueSpan1.innerText = '-';
        nQueueSpan2.innerText = '-';
    }
    function handleError (e) {
        // setTimeout(connect,1000)
        connection = false;
        feeButtons.forEach((button) => {
            button.disabled = true;
            button.style.backgroundColor='';
        });
        feeConnectionButton.disabled = false;
        delete ws;
        ws = null;
        dataRateSpan1.innerText = '-';
        dataRateSpan2.innerText = '-';
        nTaskSpan1.innerText = '-';
        nTaskSpan2.innerText = '-';
        nQueueSpan1.innerText = '-';
        nQueueSpan2.innerText = '-';
    }
    function handleMessage (e) {
        var rsp = e.data.split(" ");
        if(rsp[0]=="dataRate1"){
            dataRateSpan1.innerText = parseFloat(rsp[1]).toFixed(2);
        }
        //else if(rsp[0]=="initSitcp"){
        //    if(rsp[1]=='Done'){
        //        console.log("initSitcp Done");
        //    }
        //}
        //else if(rsp[0]=="shutdownSiTCP"){
        //    if(rsp[1]=='Done'){
        //        console.log("shutdownSiTCP Done");
        //    }
        //}
        else if(rsp[0]=="SiTCPState"){
            if(parseInt(rsp[1])==-1 && parseInt(rsp[2])==-1){
                feeInitButton.style.backgroundColor = 'yellow';
		feeShutdownButton.style.backgroundColor='';
                feeStartButton.style.backgroundColor = '';
                feeStopButton.style.backgroundColor = '';	
                feeInitButton.disabled=false;
		feeShutdownButton.disabled=true;
                feeStartButton.disabled = true;
		feeStopButton.disabled=true;
            }else if(parseInt(rsp[1])==0 && parseInt(rsp[2])==0){
                feeInitButton.style.backgroundColor = '#00FF00';
		feeShutdownButton.style.backgroundColor='';
                feeStartButton.style.backgroundColor = 'yellow';
                feeStopButton.style.backgroundColor = '';	
                feeInitButton.disabled=true;
		feeShutdownButton.disabled=false;
                feeStartButton.disabled = false;
		feeStopButton.disabled=true;
            }else if(parseInt(rsp[1])==2 && parseInt(rsp[2])==2){
                feeInitButton.style.backgroundColor = '#00FF00';
		feeShutdownButton.style.backgroundColor='';
                feeStartButton.style.backgroundColor = '#00FF00';
                feeStopButton.style.backgroundColor = '';	
                feeInitButton.disabled=true;
		feeShutdownButton.disabled=true;
                feeStartButton.disabled = true;
		feeStopButton.disabled=false;
            }
            else if(parseInt(rsp[1])==4 && parseInt(rsp[2])==4){
                feeInitButton.style.backgroundColor = '#00FF00';
		feeShutdownButton.style.backgroundColor='yellow';
                feeStartButton.style.backgroundColor = '';
                feeStopButton.style.backgroundColor = 'red';	
                feeInitButton.disabled=true;
		feeShutdownButton.disabled=false;
                feeStartButton.disabled = true;
		feeStopButton.disabled=true;
            }
            // console.log(rsp);
        }
        else if(rsp[0]=="SiTCPDataAcquisitionState"){
            if(parseInt(rsp[1])==2 && parseInt(rsp[2])==2){
                feeDataStartButton.style.backgroundColor = '#00FF00';
                feeDataStartButton.disabled = true;
            }else{
                feeDataStartButton.style.backgroundColor = '';
                feeDataStartButton.disabled = false;
            }
        }
        else if(rsp[0]=="SiTCPDecoderState"){
            if(parseInt(rsp[1])==2 && parseInt(rsp[2])==2){
                decoderStartButton.style.backgroundColor = '#00FF00';
                decoderStartButton.disabled = true;
            }else{
                decoderStartButton.style.backgroundColor = '';
                decoderStartButton.disabled = false;
                decoderStopButton.style.backgroundColor = '';
            }
        }

        else if(rsp[0]=="dataRate2"){
            dataRateSpan2.innerText = parseFloat(rsp[1]).toFixed(2);
        }
        else if(rsp[0]=="nTask1"){
            nTaskSpan1.innerText = parseInt(rsp[1]);
        }
        else if(rsp[0]=="nTask2"){
            nTaskSpan2.innerText = parseInt(rsp[1]);
        }
        else if(rsp[0]=="nQueue1"){
            nQueueSpan1.innerText = parseInt(rsp[1]);
        }
        else if(rsp[0]=="nQueue2"){
            nQueueSpan2.innerText = parseInt(rsp[1]);
        }
        else if(rsp[0]=="nositcp1"){
            feeInitButton.disabled=false;
            dataRateSpan1.innerText = '-';
        }
        else if(rsp[0]=="nositcp2"){
            feeInitButton.disabled=false;
            dataRateSpan2.innerText = '-';
        }
        else if(rsp[0]=="decoderState1"){
            if(rsp[1]=='False'){
                decoderStartButton.style.backgroundColor='';
            }else{
                decoderStartButton.style.backgroundColor='#00FF00';
            }
        }
        else if(rsp[0]=="daqState1"){
            feeInitButton.style.backgroundColor = '#00FF00';
            feeInitButton.disabled=true;
            if(parseInt(rsp[1])==2){
                feeStartButton.style.backgroundColor='#00FF00';
            }else{
                feeStartButton.style.backgroundColor='';
            }
        }
        else if(rsp[0]=="decoderState2"){
            if(rsp[1]=='False'){
                decoderStartButton.style.backgroundColor='';
            }else{
                decoderStartButton.style.backgroundColor='#00FF00';
            }
        }
        else if(rsp[0]=="daqState2"){
            feeInitButton.style.backgroundColor = '#00FF00';
            feeInitButton.disabled=true;
            if(parseInt(rsp[1])==2){
                feeStartButton.style.backgroundColor='#00FF00';
            }else{
                feeStartButton.style.backgroundColor='';
            }
        }
        if(rsp[0]=="selftrigger"){
            feeselfhitButton.disabled = false;
            feeselfhitButton.style.backgroundColor = "yellow";
            alert("噪声测试配置完成");
        }
        else if(rsp[0]=="thresholdsetting"){
            feeSpanThresholdProcess.innerText = rsp[1];
            feesetupthresholdButton.disabled = false;
            feesetupthresholdButton.style.backgroundColor = '#00FF00';
        }
        else if(rsp[0]=="T0"){
            T0_Button.disabled = false;
            T0_Button.style.backgroundColor = '#00FF00';
            alert("T0参数已设置");
        }
        else if(rsp[0]=="setfeenchannel"){
            nchannelInput.disabled = false;
            nchannelInput.backgroundColor = '#00FF00';
        }
        else if(rsp[0]=="setfeeslope"){
            slopthresholdInput.disabled = false;
            slopthresholdInput.backgroundColor = '#00FF00';
        }
        else if(rsp[0]=="setBufferFileSize"){
            bufferFileSizeInput.disabled = false;
        }
    }
    function dump(){
        // wsSend('dumpSitcp');
        wsSend('getSiTCPState');
        wsSend('getSiTCPDataAcquisitionState');
        wsSend('getSiTCPDecoderState');
        wsSend('getSiTCP1DataRate');
        wsSend('getSiTCP2DataRate');
        wsSend('getSiTCP1NTask');
        wsSend('getSiTCP2NTask');
        wsSend('getSiTCP1NQueue');
        wsSend('getSiTCP2NQueue');
    }
    // connect();
    setInterval(dump, 1000);
    init() 
})(document);

((doc) => {
    //===========================RawEventProcessor部分===========================
    // RawEventProcessor的按钮并禁用
    const RawEventProcessorButtons = doc.querySelectorAll('#RawEventProcessor button');
    RawEventProcessorButtons.forEach((button) => {
        button.disabled = true;
    });
    const DPConnectionButton = doc.querySelector('#DP_connect');
    DPConnectionButton.disabled=false;
    const DPInitButton = doc.querySelector('#DP_init');
    const DPShutdownButton = doc.querySelector('#DP_shutdown');

    const DPRateIntervalInput = doc.querySelector('#DP_RateIntervalInput');
    DPRateIntervalInput.value = '60000';
    const DPRateIntervalButton = doc.querySelector('#DP_RateInterval');

    const eventsPerFile = doc.querySelector('#eventsPerFile');
    eventsPerFile.value = 1000;
    const storageDir = doc.querySelector('#storageDir');
    storageDir.value = './output'
    // event 显示
    const DPTotalEventSpan = doc.querySelector('#DPTotalEvent');
    const DPCurrentEventIDSpan = doc.querySelector('#DPCurrentEventID');
    const DPEventRateSpan = doc.querySelector('#DPEventRate')
    DPEventRateSpan.innerText = '-';
    //
    const DPNTaskSpan = doc.querySelector('#DPNTasks');
    DPNTaskSpan.innerText = '-';
    // 设置主机地址的默认值
    const hostAddressInput = doc.querySelector('#Host_IP');
    // 设置端口的默认值
    const hostPortInput = doc.querySelector('#Host_Port');

    const RawEventProcessorStartButton = doc.querySelector('#DP_start');
    const RawEventProcessorStopButton = doc.querySelector('#DP_stop');

    const RawEventStartButton = doc.querySelector('#DP_start_rawevent');
    const RawEventStopButton = doc.querySelector('#DP_stop_rawevent');
    const EventStartButton = doc.querySelector('#DP_start_event');
    const EventStopButton = doc.querySelector('#DP_stop_event');

    const outputFileIDSpan = doc.querySelector('#outputFileID');

    const ParameterButton = doc.querySelector('#DP_ParameterButton');
    const ParameterEventsInput = doc.querySelector('#DP_ParameterEventsInput');
    ParameterEventsInput.value = 100;
    const ParaterEventsSpan = doc.querySelector('#DP_ParameterEvents');
    ParaterEventsSpan.innerText = "";

    const WvaluelInput = doc.querySelector('#RawEventProcessor input[placeholder="Wvalue"]');
    WvaluelInput.value = 30.0;
    const VdriftlInput = doc.querySelector('#RawEventProcessor input[placeholder="Vdrift"]');
    VdriftlInput.value = 0.5;
    const FEE00_01Input = doc.querySelector('#FEE00_01');
    FEE00_01Input.value = 14;
    const FEE02_03Input = doc.querySelector('#FEE02_03');
    FEE02_03Input.value = 15;
    const FEE04_05Input = doc.querySelector('#FEE04_05');
    FEE04_05Input.value = 7;
    const FEE06_07Input = doc.querySelector('#FEE06_07');
    FEE06_07Input.value = 6;
    const FEE08_09Input = doc.querySelector('#FEE08_09');
    FEE08_09Input.value = 11;
    const FEE10_11Input = doc.querySelector('#FEE10_11');
    FEE10_11Input.value = 10;
    const FEE12_13Input = doc.querySelector('#FEE12_13');
    FEE12_13Input.value = 2;
    const FEE14_15Input = doc.querySelector('#FEE14_15');
    FEE14_15Input.value = 3;
    const FEE16_17Input = doc.querySelector('#FEE16_17');
    FEE16_17Input.value = 5;
    const FEE18_19Input = doc.querySelector('#FEE18_19');
    FEE18_19Input.value = 4;
    const FEE20_21Input = doc.querySelector('#FEE20_21');
    FEE20_21Input.value = 12;
    const FEE22_23Input = doc.querySelector('#FEE22_23');
    FEE22_23Input.value = 13;
    const FEE24_25Input = doc.querySelector('#FEE24_25');
    FEE24_25Input.value = 9;
    const FEE26_27Input = doc.querySelector('#FEE26_27');
    FEE26_27Input.value = 8;
    const FEE28_29Input = doc.querySelector('#FEE28_29');
    FEE28_29Input.value = 0;
    const FEE30_31Input = doc.querySelector('#FEE30_31');
    FEE30_31Input.value = 1;
    const ElectrnicGainFilePathInput = doc.querySelector('#RawEventProcessor input[type="text"][placeholder="电子学增益配置文件"]');
    ElectrnicGainFilePathInput.value = '/storage/ATTPC/DAQ/Electronic_parameter.json';
    const MicromegasGasFilePathInput = doc.querySelector('#RawEventProcessor input[type="text"][placeholder="阳极板增益配置文件"]');
    MicromegasGasFilePathInput.value = 'MicromegasFilePath';

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
        DPConnectionButton.addEventListener('click',connect,false);
        DPInitButton.addEventListener('click',DPInit,false);
        DPShutdownButton.addEventListener('click', DPShutdown, false);
        RawEventProcessorStartButton.addEventListener('click',RawEventProcessorStart,false);
        RawEventProcessorStopButton.addEventListener('click',RawEventProcessorStop,false);
        eventsPerFile.addEventListener('keydown', setNRawEventsPerFile, false);
        storageDir.addEventListener('keydown', setRawEventFilePath, false);

        RawEventStartButton.addEventListener('click', turnOnRawEventSave, false);
        RawEventStopButton.addEventListener('click',turnOffRawEventSave,false);
        EventStartButton.addEventListener('click',turnOnEventSave,false);
        EventStopButton.addEventListener('click',turnOffEventSave,false);
        ParameterButton.addEventListener('click',parameterGenerate,false);
        DPRateIntervalButton.addEventListener('click',SetDPRateInterval,false)
    }
    function SetDPRateInterval(e){
        clearInterval(DPRateIntervalID);
        DPRateIntervalID = setInterval(estimateEventRate, parseInt(DPRateIntervalInput.value));
    }
    function parameterGenerate(){
        wsSend('setDataProcessorParameterEvents ' + ParameterEventsInput.value);
        ParameterButton.disabled = true;
    }
    function turnOnRawEventSave(){
        wsSend('turnOnRawEventSave');
        RawEventStartButton.disabled = true;
        RawEventStopButton.disabled = false;
    }
    function turnOffRawEventSave(){
        wsSend('turnOffRawEventSave')
        RawEventStartButton.disabled = false;
        RawEventStopButton.disabled = true;
    }
    function turnOnEventSave(){
        wsSend('turnOnEventSave')
        EventStartButton.disabled = true;
        EventStopButton.disabled = false;
    }
    function turnOffEventSave(){
        wsSend('turnOffEventSave')
        EventStartButton.disabled = false;
        EventStopButton.disabled = true;
    }
    function setRawEventFilePath(e){
        if(e.key==='Enter'){
            const path = storageDir.value;
            const command = `setDataProcessorDir ${path}`;
            wsSend(command);
        }
    }
    function setNRawEventsPerFile(e){
        if(e.key==='Enter'){
            const NEvents = eventsPerFile.value;
            const command = `setDataProcessorEventsPerFile ${NEvents}`;
            wsSend(command);
        }
    }
    function RawEventProcessorStop(){
        wsSend('stopDataProcessor');
        RawEventProcessorStartButton.disabled = false;
        RawEventProcessorStopButton.disabled = true;
        ParameterButton.disabled = true;
    }
    function RawEventProcessorStart(){
        wsSend('startDataProcessor');
        RawEventProcessorStartButton.disabled = true;
        RawEventProcessorStopButton.disabled = false;
        ParameterButton.disabled = false;
    }
    function RawEventProcessorInit(){
        
        const command = `initDataProcessor ${storageDir.value} ${eventsPerFile.value} `;
        wsSend(command)

        RawEventProcessorStartButton.disabled = false;
        RawEventProcessorStopButton.disabled = false;

        RawEventStartButton.disabled = false;
        EventStartButton.disabled = false;

        // if (RawEventFilePathInput.value !== ''){
        //     const path = RawEventFilePathInput.value;
        //     const command = `setDataProcessDir ${path}`;
        //     wsSend(command);
        // }
        // if (NRawEventsPerFileInput.value !== ''){
        //     const NEvents = NRawEventsPerFileInput.value;
        //     const command = `setEventsPerFile ${NEvents}`;
        //     wsSend(command);
        // }

        const command1 = `setWvalueAndVdrift ${WvaluelInput.value} ${VdriftlInput.value}`;
        wsSend(command1);
        const command2 = `setGainFile ${ElectrnicGainFilePathInput.value} ${MicromegasGasFilePathInput.value}`;
        // console.log('command2',command2)
        wsSend(command2);
        const FPC2 = [
            {"0": FEE00_01Input.value},
            {"1": FEE00_01Input.value},
            {"2": FEE02_03Input.value},
            {"3": FEE02_03Input.value},
            {"4": FEE04_05Input.value},
            {"5": FEE04_05Input.value},
            {"6": FEE06_07Input.value},
            {"7": FEE06_07Input.value},
            {"8": FEE08_09Input.value},
            {"9": FEE08_09Input.value},
            {"10": FEE10_11Input.value},
            {"11": FEE10_11Input.value},
            {"12": FEE12_13Input.value},
            {"13": FEE12_13Input.value},
            {"14": FEE14_15Input.value},
            {"15": FEE14_15Input.value},
            {"16": FEE16_17Input.value},
            {"17": FEE16_17Input.value},
            {"18": FEE18_19Input.value},
            {"19": FEE18_19Input.value},
            {"20": FEE20_21Input.value},
            {"21": FEE20_21Input.value},
            {"22": FEE22_23Input.value},
            {"23": FEE22_23Input.value},
            {"24": FEE24_25Input.value},
            {"25": FEE24_25Input.value},
            {"26": FEE26_27Input.value},
            {"27": FEE26_27Input.value},
            {"28": FEE28_29Input.value},
            {"29": FEE28_29Input.value},
            {"30": FEE30_31Input.value},
            {"31": FEE30_31Input.value}
        ];        
        // const jsonString = JSON.stringify(FPC2);
        const jsonString = JSON.stringify(FPC2, null, 0);
        // console.log('jsonString',jsonString)
        const command3 = `setFPC2 ${jsonString}`
        // console.log('command3',command3)
        wsSend(command3)
            
    }
    function DPShutdown(e){
        wsSend('shutdownDataProcessor')
    }
    function DPInit(e){
        RawEventProcessorInit();
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
        RawEventProcessorButtons.forEach((button) => {
            button.disabled = false;
        });
        DPConnectionButton.style.backgroundColor = '#00FF00';
        DPConnectionButton.disabled=true;
        DPInitButton.style.backgroundColor = 'yellow';
        wsSend('register')
    }
    function handleClose (e) {
        connection = false;
        RawEventProcessorButtons.forEach((button) => {
            button.disabled = true;
            button.style.backgroundColor='';
        });
        DPConnectionButton.disabled=false;
        delete ws;
        ws = null;
    }
    function handleError (e) {
        // setTimeout(connect,3000)
        connection = false;
        RawEventProcessorButtons.forEach((button) => {
            button.disabled = true;
            button.style.backgroundColor='';
        });
        DPConnectionButton.disabled=false;
        delete ws;
        ws = null;
    }
    function handleMessage (e) {
        var rsp = e.data.split(" ");
        if(rsp[0]=="DataProcessorTotalEvent"){
            DPTotalEventSpan.innerText = parseInt(rsp[1]);
        }
        else if(rsp[0]=="DataProcessorCurrentEventID"){
            DPCurrentEventIDSpan.innerText = parseInt(rsp[1]);
        }
        else if(rsp[0]=="DataProcessorParameterEvents"){
            ParaterEventsSpan.innerText = parseInt(rsp[1]);
            if(parseInt(rsp[1]) == 0) {
                // alert("参数文件已产生");
                ParameterButton.disabled = false;
            }
        }
        else if(rsp[0]=="DataProcessorNTask"){
            //
            DPNTaskSpan.innerText = parseInt(rsp[1]);
        }
        else if(rsp[0]=="DataProcessorCurrentFileID"){
            outputFileIDSpan.innerText = parseInt(rsp[1]);
        }
        else if(rsp[0]=="DataProcessorState"){
            if(parseInt(rsp[1]) == -1){
                DPInitButton.style.backgroundColor='yellow'
		DPShutdownButton.style.backgroundColor=''
		RawEventProcessorStartButton.style.backgroundColor=''
                RawEventProcessorStopButton.style.backgroundColor=''
                DPInitButton.disabled = false;
		DPShutdownButton.disabled = true;
		RawEventProcessorStartButton.disabled = true;
                RawEventProcessorStopButton.disabled = true;		
            }else if(parseInt(rsp[1]) == 0){
                DPInitButton.style.backgroundColor='#00FF00'
		DPShutdownButton.style.backgroundColor=''
		RawEventProcessorStartButton.style.backgroundColor='yellow'
                RawEventProcessorStopButton.style.backgroundColor=''
                DPInitButton.disabled = true;
		DPShutdownButton.disabled = false;
		RawEventProcessorStartButton.disabled = false;
                RawEventProcessorStopButton.disabled = true;		
            }
            if(parseInt(rsp[1]) == 2){
                DPInitButton.style.backgroundColor='#00FF00'
		DPShutdownButton.style.backgroundColor=''
		RawEventProcessorStartButton.style.backgroundColor='#00FF00'
                RawEventProcessorStopButton.style.backgroundColor=''
                DPInitButton.disabled = true;
		DPShutdownButton.disabled = true;
		RawEventProcessorStartButton.disabled = true;
                RawEventProcessorStopButton.disabled = false;		
            }
            if(parseInt(rsp[1]) == 4){
                DPInitButton.style.backgroundColor='#00FF00'
		DPShutdownButton.style.backgroundColor='yellow'
		RawEventProcessorStartButton.style.backgroundColor=''
                RawEventProcessorStopButton.style.backgroundColor='red'
                DPInitButton.disabled = true;
		DPShutdownButton.disabled = false;
		RawEventProcessorStartButton.disabled = true;
                RawEventProcessorStopButton.disabled = true;		
            }
        }
    }

    function dump(){
        // wsSend('dumpDP');
        wsSend('getDataProcessorState');
        wsSend('getDataProcessorTotalEvent');
        wsSend('getDataProcessorCurrentEventID');
        wsSend('getDataProcessorCurrentFileID');
        wsSend('getDataProcessorParameterEvents');
        wsSend('getDataProcessorNTask')
    }
    // connect();
    setInterval(dump, 1000);
    var DPEventNumber = 0;
    function estimateEventRate(){
        DPCurrentEventNumber=parseInt(DPTotalEventSpan.innerText);
        if(DPEventNumber>0){
            rate = (DPCurrentEventNumber-DPEventNumber)/60;
            DPEventRateSpan.innerText = parseFloat(rate).toFixed(2);
        }
        DPEventNumber= DPCurrentEventNumber;
    }
    QARateIntervalID = setInterval(estimateEventRate, 60000);
    init()
})(document);


((doc) => {
    //===========================QAProcessor部分===========================
    // QAProcessor的按钮并禁用
    const Buttons = doc.querySelectorAll('#QAProcessor button');
    Buttons.forEach((button) => {
        button.disabled = true;
    });
    const ConnectionButton = doc.querySelector('#QA_connect');
    ConnectionButton.disabled = false;
    const InitButton = doc.querySelector('#QA_init');
    const ShutdownButton = doc.querySelector('#QA_shutdown')
    const StartButton = doc.querySelector('#QA_start');
    const StopButton = doc.querySelector('#QA_stop');
    const QATotalEventSpan = doc.querySelector('#QATotalEvent');
    const QACurrentEventIDSpan = doc.querySelector('#QACurrentEventID');
    const QAEventRateSpan = doc.querySelector('#QAEventRate');
    QAEventRateSpan.innerText='-';
    const hostAddressInput = doc.querySelector('#Host_IP');
    const hostPortInput = doc.querySelector('#Host_Port');
    const HttpServerPort = document.querySelector('#HttpServerPort');
    HttpServerPort.value = '8008';
    const ClearPlotsButton = doc.querySelector('#QA_ClearPlots');
    const QAEventParametersInput = doc.querySelector('#QA_EventParameter');
    QAEventParametersInput.value = './eventParameters.json'
    const QARateIntervalInput = doc.querySelector('#QA_RateIntervalInput');
    QARateIntervalInput.value = '60000';
    const QARateIntervalButton = doc.querySelector('#QA_RateInterval');

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
            //console.log('userNamepassWord', userName, passWord)
        }
	    bindEvent()
    }
    function bindEvent() {
        ConnectionButton.addEventListener('click',connect,false);
        InitButton.addEventListener('click',QAProcessorInit,false);
        ShutdownButton.addEventListener('click',QAProcessorShutdown,false);
        StartButton.addEventListener('click',QAProcessorStart,false);
        StopButton.addEventListener('click',QAProcessorStop,false);

        ClearPlotsButton.addEventListener('click',ClearPlots,false);
        QARateIntervalButton.addEventListener('click',SetQARateInterval,false)
    }
    function SetQARateInterval(e){
        clearInterval(QARateIntervalID);
        QARateIntervalID = setInterval(estimateEventRate, parseInt(QARateIntervalInput.value));
    }
    function ClearPlots(e){
        wsSend('OnlineQAClearPlots');
    }

    function QAProcessorShutdown(e){
        wsSend('shutdownOnlineQA');
    }
    function QAProcessorStop(){
        wsSend('stopOnlineQA');
    }
    function QAProcessorStart(){
        wsSend('startOnlineQA');
        setTimeout(reloadQAFram, 2000);
    }
    function reloadQAFram(){
        var iframe = doc.querySelector('#QAFrame');
        iframe.src = 'http://'+hostAddressInput.value+':'+HttpServerPort.value;
    }
    function QAProcessorInit(e){
        const command = `initOnlineQA ${HttpServerPort.value} ${QAEventParametersInput.value}`;
        wsSend(command)
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
        // setTimeout(connect,3000)
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
	    // console.log('message', e);
        var rsp = e.data.split(" ");
        if(rsp[0]=="OnlineQATotalEvent"){
            QATotalEventSpan.innerText = parseInt(rsp[1]);
        }
        else if(rsp[0]=="OnlineQACurrentEventID"){
            QACurrentEventIDSpan.innerText = parseInt(rsp[1]);
        }
        else if(rsp[0] == "OnlineQAState"){
            if(parseInt(rsp[1]) == -1){
                InitButton.style.backgroundColor = 'yellow';
		        ShutdownButton.style.backgroundColor = '';
                StartButton.style.backgroundColor = '';
                StopButton.style.backgroundColor = '';

                InitButton.disabled = false;
		        ShutdownButton.disabled = true;
                StartButton.disabled = true;
                StopButton.disabled = true;
            }else if(parseInt(rsp[1]) == 0){
                InitButton.style.backgroundColor = '#00FF00';
		        ShutdownButton.style.backgroundColor = '';
                StartButton.style.backgroundColor = 'yellow';
                StopButton.style.backgroundColor = '';
                
                InitButton.disabled = true;
		        ShutdownButton.disabled = false;
                StartButton.disabled = false;
                StopButton.disabled = true;
            }else if(parseInt(rsp[1]) == 2){
                InitButton.style.backgroundColor = '#00FF00';
		        ShutdownButton.style.backgroundColor = '';
                StartButton.style.backgroundColor = '#00FF00';
                StopButton.style.backgroundColor = '';

                InitButton.disabled = true;
		        ShutdownButton.disabled = true;
                StartButton.disabled = true;
                StopButton.disabled = false;

                //setTimeout(reloadQAFram, 2000);
            }else if(parseInt(rsp[1]) == 4){
                InitButton.style.backgroundColor = '#00FF00';
		        ShutdownButton.style.backgroundColor = 'yellow';
                StartButton.style.backgroundColor = '';
                StopButton.style.backgroundColor = 'red';

                InitButton.disabled = true;
		        ShutdownButton.disabled = false;
                StartButton.disabled = true;
                StopButton.disabled = true;
            }
        }
    }
    function dump(){
        wsSend('getOnlineQAState');
        wsSend('getOnlineQATotalEvent');
        wsSend('getOnlineQACurrentEventID')
    }
    setInterval(dump, 1000);

    var QAEventNumber = 0;
    function estimateEventRate(){
        QACurrentEventNumber=parseInt(QATotalEventSpan.innerText);
        if(QAEventNumber>0){
            rate = (QACurrentEventNumber-QAEventNumber)/60;
            QAEventRateSpan.innerText = parseFloat(rate).toFixed(2);
        }
        QAEventNumber= QACurrentEventNumber;
    }
    QARateIntervalID = setInterval(estimateEventRate, 60000);

    init()

})(document);

((doc) => {
    //===========================Log部分===========================
    const connectionButton = doc.querySelector('#log_connect');
    const createLogButton = doc.querySelector('#createLog');
    createLogButton.disabled = true;
    const hostAddressInput = doc.querySelector('#Host_IP');
    const hostPortInput = doc.querySelector('#Host_Port');
    const logs = doc.querySelector('#logs');
    logs.style.height = '300px';
    logs.disabled = true;
    const logInput = doc.querySelector('#logInput');
    logInput.style.height = '100px';
    const addLogItermButton = doc.querySelector('#addLogIterm');
    const logDir = doc.querySelector('#storageDir');

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
        connectionButton.addEventListener('click',connect,false);
        createLogButton.addEventListener('click',createLog,false);
        addLogItermButton.addEventListener('click',addLogIterm,false);
    }
    function createLog(e){
        createLogButton.disabled = true;
        createLogButton.style.backgroundColor = '';
        wsSend('newLog '+ logDir.value);
    }
    function addLogIterm(e){
        wsSend('log '+ logInput.value);
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
        connectionButton.style.backgroundColor = '#00FF00';
        connectionButton.disabled = true;
        createLogButton.style.backgroundColor = '';
        createLogButton.disabled = false;
	wsSend('register')
    }
    function handleClose (e) {
        connection = false;
        connectionButton.style.backgroundColor = '';
        connectionButton.disabled = false;
        createLogButton.style.backgroundColor = '';
        createLogButton.disabled = true;
        delete ws;
        ws = null;
    }
    function handleError (e) {
        connection = false;
        connectionButton.style.backgroundColor = '';
        connectionButton.disabled = false;
        createLogButton.style.backgroundColor = '';
        createLogButton.disabled = true;
        delete ws;
        ws = null;
    }
    function handleMessage (e) {
        var rsp = e.data.split(" ");
        // console.log(rsp);
        if(rsp[0] == "newLog"){
            createLogButton.disabled = false;
            createLogButton.style.backgroundColor = '#00FF00';
        }
        else if(rsp[3] == "INFO"){
            logs.value = e.data;
            logs.scrollTop = logs.scrollHeight;
        }
        else if(rsp[0] == "noLogger"){
            createLogButton.style.backgroundColor = 'yellow';
            createLogButton.disabled = false;
        }
    }
    function dump(){
        wsSend('getLog');
    }
    setInterval(dump, 1000);
    init()
})(document);
