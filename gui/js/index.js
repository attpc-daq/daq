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
                frame.style.height = "760px";
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
        shutdownButton.style.backgroundColor = 'red';
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
            shutdownButton.style.backgroundColor = '';
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
    // feeConnectionButton.disabled = false;
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
    nTaskSpan1.value = '';
    const dataRateSpan1 = doc.querySelector('#DAQ1_dataRate');
    dataRateSpan1.value = '';
    const nTaskSpan2 = doc.querySelector('#DAQ2_nTask');
    nTaskSpan2.value = '';
    const dataRateSpan2 = doc.querySelector('#DAQ2_dataRate');
    dataRateSpan2.value = '';
    const nQueueSpan1 = doc.querySelector('#DAQ1_nQueue');
    nQueueSpan1.value = '';
    const nQueueSpan2 = doc.querySelector('#DAQ2_nQueue');
    nQueueSpan2.value = '';

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
    
    SiTCPState = -2;
    SiTCPDataAcquisitionState = -2;
    SiTCPDecoderState = -2;

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
        // feesetupthresholdButton.disabled = true;
        feesetupthresholdButton.style.backgroundColor = 'gray';
    }
    
    function feeselfhit(e) {
        wsSend('selftrigger');
        // feeselfhitButton.disabled = true;
        feeselfhitButton.style.backgroundColor = "gray";
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
        feeStopButton.backgroundColor = "gray";
        // feeStartButton.disabled = false;
    }
    function feeStart(e) {
        wsSend('startSiTCP');
        feeStartButton.disabled = true;
        feeStartButton.backgroundColor = "gray";
        // feeStopButton.disabled = false;
    }
    function feeDataStop(e) {
        wsSend('stopSiTCPData');
        feeDataStopButton.disabled = true;
        feeDataStopButton.backgroundColor = "gray";
        // feeDataStartButton.disabled = false;
    }
    function feeDataStart(e) {
        wsSend('startSiTCPData');
        feeDataStartButton.disabled = true;
        feeDataStartButton.backgroundColor = "gray";
        // feeDataStopButton.disabled = false;
    }
    function decoderStart(){
        wsSend('startSiTCPDecoder');
        decoderStartButton.disabled = true;
        decoderStartButton.backgroundColor = "gray";
    }
    function decoderStop(){
        wsSend('stopSiTCPDecoder');
        decoderStopButton.disabled = true;
        decoderStopButton.backgroundColor = "gray";
    }
    function feeInit(e){
        const command = `initSitcp ${siTcpAddress_1_Input.value} ${siTcpport_1_Input.value} ${bufferFilePathInput1.value} ${siTcpAddress_2_Input.value} ${siTcpport_2_Input.value} ${bufferFilePathInput2.value} ${bufferFileSizeInput.value}`;
        wsSend(command)
        feeInitButton.disabled = true;
        feeInitButton.style.backgroundColor = 'gray';
    }
    function feeShutdown(e){
        wsSend('shutdownSiTCP');
        feeShutdownButton.disabled = true;
        feeShutdownButton.style.backgroundColor='gray';
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
        feeConnectionButton.backgroundColor = "gray";
    }
    function wsSend (msg) {
        if(!connection) return;
        if(!ws) return;
        if (!msg.trim().length) return;
        ws.send(msg);
    }
    function handleOpen (e) {
        connection = true;
        SiTCPState = -1;
        updateGUI();
        wsSend('register')
    }
    function handleClose (e) {
        connection = false;
        SiTCPState = -2;
        delete ws;
        ws = null;
        updateGUI();
    }
    function handleError (e) {
        connection = false;
        SiTCPState = -2;
        delete ws;
        ws = null;
        updateGUI();
    }
    function handleMessage (e) {
        var rsp = e.data.split(" ");
        if(rsp[0]=="SiTCPState"){
            if(parseInt(rsp[1])==-1 && parseInt(rsp[2])==-1){
                SiTCPState = -1;
                updateGUI();
            }else if(parseInt(rsp[1])==0 && parseInt(rsp[2])==0){
                SiTCPState = 0;
                updateGUI();
            }else if(parseInt(rsp[1])==2 && parseInt(rsp[2])==2){
                SiTCPState = 2;
                updateGUI();
            }
            else if(parseInt(rsp[1])==4 && parseInt(rsp[2])==4){
                SiTCPState = 4;
                updateGUI();
            }
            // console.log(rsp);
        }
        else if(rsp[0]=="SiTCPDataAcquisitionState"){
            if(parseInt(rsp[1])==0 && parseInt(rsp[2])==0){
                SiTCPDataAcquisitionState = 0;
                updateGUI();
            }else if(parseInt(rsp[1])==2 && parseInt(rsp[2])==2){
                SiTCPDataAcquisitionState = 2;
                updateGUI();
            }else if(parseInt(rsp[1])==4 && parseInt(rsp[2])==4){
                SiTCPDataAcquisitionState = 4;
                updateGUI();
            }
        }
        else if(rsp[0]=="SiTCPDecoderState"){
            if(parseInt(rsp[1])==0 && parseInt(rsp[2])==0){
                SiTCPDecoderState = 0;
                updateGUI();
            }else if(parseInt(rsp[1])==2 && parseInt(rsp[2])==2){
                SiTCPDecoderState = 2;
                updateGUI();
            }else if(parseInt(rsp[1])==4 && parseInt(rsp[2])==4){
                SiTCPDecoderState = 4;
                updateGUI();
            }
        }else if(rsp[0]=="dataRate1"){
            dataRateSpan1.value = parseFloat(rsp[1]).toFixed(2);
            if(dataRateSpan1.value == 0){
                dataRateSpan1.style.backgroundColor = "red";
            }else if(dataRateSpan1.value > 0){
                dataRateSpan1.style.backgroundColor = "blue";
            }else{
                dataRateSpan1.style.backgroundColor = "gray";
            }
        }else if(rsp[0]=="dataRate2"){
            dataRateSpan2.value = parseFloat(rsp[1]).toFixed(2);
            if(dataRateSpan2.value == 0){
                dataRateSpan2.style.backgroundColor = "red";
            }else if(dataRateSpan2.value > 0){
                dataRateSpan2.style.backgroundColor = "blue";
            }else{
                dataRateSpan2.style.backgroundColor = "gray";
            }
        }
        else if(rsp[0]=="nTask1"){
            nTaskSpan1.value = parseInt(rsp[1]);
            if(nTaskSpan1.value == 0){
                nTaskSpan1.style.backgroundColor = "red";
            }else if(nTaskSpan1.value > 0){
                nTaskSpan1.style.backgroundColor = "blue";
            }else{
                nTaskSpan1.style.backgroundColor = "gray";
            }
        }
        else if(rsp[0]=="nTask2"){
            nTaskSpan2.value = parseInt(rsp[1]);
            if(nTaskSpan2.value == 0){
                nTaskSpan2.style.backgroundColor = "red";
            }else if(nTaskSpan2.value > 0){
                nTaskSpan2.style.backgroundColor = "blue";
            }else{
                nTaskSpan2.style.backgroundColor = "gray";
            }
        }
        else if(rsp[0]=="nQueue1"){
            nQueueSpan1.value = parseInt(rsp[1]);
            if(nQueueSpan1.value == 0){
                nQueueSpan1.style.backgroundColor = "red";
            }else if(nQueueSpan1.value > 0){
                nQueueSpan1.style.backgroundColor = "blue";
            }else{
                nQueueSpan1.style.backgroundColor = "gray";
            }
        }
        else if(rsp[0]=="nQueue2"){
            nQueueSpan2.value = parseInt(rsp[1]);
            if(nQueueSpan2.value == 0){
                nQueueSpan2.style.backgroundColor = "red";
            }else if(nQueueSpan2.value > 0){
                nQueueSpan2.style.backgroundColor = "blue";
            }else{
                nQueueSpan2.style.backgroundColor = "gray";
            }
        }
        if(rsp[0]=="selftrigger"){
            // feeselfhitButton.disabled = false;
            feeselfhitButton.style.backgroundColor = "#00FF00";
            alert("噪声测试配置完成");
        }
        else if(rsp[0]=="thresholdsetting"){
            feeSpanThresholdProcess.innerText = rsp[1];
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
    function updateGUI(){
        //feeConnectionButton
        if(SiTCPState == -2){
            feeConnectionButton.style.backgroundColor = "yellow";
            feeConnectionButton.disabled = false;
        }else if(SiTCPState > -2){
            feeConnectionButton.style.backgroundColor = "#00FF00";
            feeConnectionButton.disabled = true;
        }
        //feeInitButton
        if(SiTCPState == -2){
            feeInitButton.style.backgroundColor = "";
            feeInitButton.disabled = true;
        }else if(SiTCPState == -1){
            feeInitButton.style.backgroundColor = "yellow";
            feeInitButton.disabled = false;
        }else if(SiTCPState > -1){
            feeInitButton.style.backgroundColor = "#00FF00";
            feeInitButton.disabled = true;
        }
        //feeShutdownButton
        if(SiTCPState < 0){
            feeShutdownButton.style.backgroundColor = "";
            feeShutdownButton.disabled = true;
        }else if(SiTCPState == 0 ){
            feeShutdownButton.style.backgroundColor = "";
            feeShutdownButton.disabled = false;
        }else if(SiTCPState == 2){
            feeShutdownButton.style.backgroundColor = "";
            feeShutdownButton.disabled = true;
        }else if(SiTCPState == 4){
            feeShutdownButton.style.backgroundColor = "yellow";
            feeShutdownButton.disabled = false;
        }
        //feeStartButton
        if(SiTCPState < 0){
            feeStartButton.style.backgroundColor = "";
            feeStartButton.disabled = true;
        }else if(SiTCPState == 0){
            feeStartButton.style.backgroundColor = "yellow";
            feeStartButton.disabled = false;
        }else if(SiTCPState == 2){
            feeStartButton.style.backgroundColor = "#00FF00";
            feeStartButton.disabled = true;
        }else if(SiTCPState > 2){
            feeStartButton.style.backgroundColor = "";
            feeStartButton.disabled = true;
        }
        //feeStopButton
        if(SiTCPState <= 0){
            feeStopButton.style.backgroundColor = "";
            feeStopButton.disabled = true;
        }else if(SiTCPState == 2){
            feeStopButton.style.backgroundColor = "";
            feeStopButton.disabled = false;
        }else if(SiTCPState == 4){
            feeStopButton.style.backgroundColor = "red";
            feeStopButton.disabled = true;
        }
        //feeDataStartButton
        if(SiTCPState == 2){
            if(SiTCPDataAcquisitionState == 2){
                feeDataStartButton.style.backgroundColor = "#00FF00";
                feeDataStartButton.disabled = true;
            }else{
                feeDataStartButton.style.backgroundColor = "";
                feeDataStartButton.disabled = false;
            }
        }else{
            feeDataStartButton.style.backgroundColor = "";
            feeDataStartButton.disabled = true;
        }
        //feeDataStopButton
        if(SiTCPState == 2){
            if(SiTCPDataAcquisitionState == 2){
                feeDataStopButton.style.backgroundColor = "";
                feeDataStopButton.disabled = false;
            }else{
                feeDataStopButton.style.backgroundColor = "";
                feeDataStopButton.disabled = true;
            }
        }else{
            feeDataStopButton.style.backgroundColor = "";
            feeDataStopButton.disabled = true;
        }
        //decoderStartButton
        if(SiTCPState == 2){
            if(SiTCPDecoderState == 2){
                decoderStartButton.style.backgroundColor = "#00FF00";
                decoderStartButton.disabled = true;
            }else{
                decoderStartButton.style.backgroundColor = "";
                decoderStartButton.disabled = false;
            }
        }else{
            decoderStartButton.style.backgroundColor = "";
            decoderStartButton.disabled = true;
        }
        //decoderStopButton
        if(SiTCPState == 2){
            if(SiTCPDecoderState == 2){
                decoderStopButton.style.backgroundColor = "";
                decoderStopButton.disabled = false;
            }else{
                decoderStopButton.style.backgroundColor = "";
                decoderStopButton.disabled = true;
            }
        }else{
            decoderStopButton.style.backgroundColor = "";
            decoderStopButton.disabled = true;
        }
        //other buttons
        if(SiTCPState == 0){
            feesetupthresholdButton.disabled = false;
            feeSetupButton.disabled = false;
            feeselfhitButton.disabled = false;
        }else{
            feesetupthresholdButton.disabled = true;
            feesetupthresholdButton.style.backgroundColor = '';
            feeSetupButton.disabled = true;
            feeSetupButton.style.backgroundColor = '';
            feeselfhitButton.disabled = true;
            feeselfhitButton.style.backgroundColor = '';
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
    init();
    updateGUI();
})(document);

((doc) => {
    //===========================RawEventProcessor部分===========================
    // RawEventProcessor的按钮并禁用
    const RawEventProcessorButtons = doc.querySelectorAll('#RawEventProcessor button');
    RawEventProcessorButtons.forEach((button) => {
        button.disabled = true;
    });
    const DPConnectionButton = doc.querySelector('#DP_connect');
    // DPConnectionButton.disabled=false;
    const DPInitButton = doc.querySelector('#DP_init');
    const DPShutdownButton = doc.querySelector('#DP_shutdown');

    const DPRateIntervalInput = doc.querySelector('#DP_RateIntervalInput');
    DPRateIntervalInput.value = '10';
    const DPRateIntervalButton = doc.querySelector('#DP_RateInterval');

    const eventsPerFile = doc.querySelector('#eventsPerFile');
    eventsPerFile.value = 1000;
    const storageDir = doc.querySelector('#storageDir');
    storageDir.value = './output'
    // event 显示
    const DPTotalEventSpan = doc.querySelector('#DPTotalEvent');
    DPTotalEventSpan.value = '';
    const DPCurrentEventIDSpan = doc.querySelector('#DPCurrentEventID');
    DPCurrentEventIDSpan.value = '';
    const DPEventRateSpan = doc.querySelector('#DPEventRate')
    DPEventRateSpan.value = '';
    const DPNTaskSpan = doc.querySelector('#DPNTasks');
    DPNTaskSpan.value = '';
    // 设置主机地址的默认值
    const hostAddressInput = doc.querySelector('#Host_IP');
    // 设置端口的默认值
    const hostPortInput = doc.querySelector('#Host_Port');

    const DPStartButton = doc.querySelector('#DP_start');
    const DPStopButton = doc.querySelector('#DP_stop');

    const RawEventStartButton = doc.querySelector('#DP_start_rawevent');
    const RawEventStopButton = doc.querySelector('#DP_stop_rawevent');
    const EventStartButton = doc.querySelector('#DP_start_event');
    const EventStopButton = doc.querySelector('#DP_stop_event');

    const outputFileIDSpan = doc.querySelector('#outputFileID');
    outputFileIDSpan.value = '';

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

    DPState = -2;

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
        DPStartButton.addEventListener('click',DPStart,false);
        DPStopButton.addEventListener('click',DPStop,false);
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
        DPRateIntervalID = setInterval(estimateEventRate, parseInt(DPRateIntervalInput.value)*1000);
        DPRateIntervalButton.disabled = true;
        DPRateIntervalButton.style.backgroundColor = "gray";
    }
    function parameterGenerate(){
        wsSend('setDataProcessorParameterEvents ' + ParameterEventsInput.value);
        ParameterButton.style.backgroundColor = "gray";
    }
    function turnOnRawEventSave(){
        wsSend('turnOnRawEventSave');
        RawEventStartButton.style.backgroundColor = 'gray';
    }
    function turnOffRawEventSave(){
        wsSend('turnOffRawEventSave')
        RawEventStopButton.style.backgroundColor = 'gray';
    }
    function turnOnEventSave(){
        wsSend('turnOnEventSave')
        EventStartButton.style.backgroundColor = 'gray';
    }
    function turnOffEventSave(){
        wsSend('turnOffEventSave')
        EventStopButton.style.backgroundColor = 'gray';
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
    function DPStop(){
        wsSend('stopDataProcessor');
        DPStopButton.disabled = true;
        DPStopButton.backgroundColor = "gray";
    }
    function DPStart(){
        wsSend('startDataProcessor');
        DPStartButton.disabled = true;
        DPStartButton.backgroundColor = "gray";
    }
    function DPInit(e){
        const command = `initDataProcessor ${storageDir.value} ${eventsPerFile.value} `;
        wsSend(command)
        DPInitButton.disabled = true;
        DPInitButton.style.backgroundColor = 'gray';

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
        DPShutdownButton.disabled = true;
        DPShutdownButton.style.backgroundColor='gray';
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
        DPConnectionButton.backgroundColor = "gray";
    }
    function wsSend (msg) {
        if(!connection) return;
        if(!ws) return;
        if (!msg.trim().length) return;
        ws.send(msg);
    }
    function handleOpen (e) {
        connection = true;
        DPState = -1;
        updateGUI();
        wsSend('register')
    }
    function handleClose (e) {
        connection = false;
        DPState = -2;
        delete ws;
        ws = null;
        updateGUI();
    }
    function handleError (e) {
        // setTimeout(connect,3000)
        connection = false;
        DPState = -2;
        delete ws;
        ws = null;
        updateGUI();
    }
    function handleMessage (e) {
        var rsp = e.data.split(" ");
        if(rsp[0]=="DataProcessorTotalEvent"){
            DPTotalEventSpan.value = parseInt(rsp[1]);
            if(DPTotalEventSpan.value == 0){
                DPTotalEventSpan.style.backgroundColor = "red";
            }else if(DPTotalEventSpan.value > 0){
                DPTotalEventSpan.style.backgroundColor = "blue";
            }else{
                DPTotalEventSpan.style.backgroundColor = "gray";
            }
        }
        else if(rsp[0]=="DataProcessorCurrentEventID"){
            DPCurrentEventIDSpan.value = parseInt(rsp[1]);
            if(DPCurrentEventIDSpan.value == 0){
                DPCurrentEventIDSpan.style.backgroundColor = "red";
            }else if(DPCurrentEventIDSpan.value > 0){
                DPCurrentEventIDSpan.style.backgroundColor = "blue";
            }else{
                DPCurrentEventIDSpan.style.backgroundColor = "gray";
            }
        }
        else if(rsp[0]=="DataProcessorParameterEvents"){
            ParaterEventsSpan.innerText = parseInt(rsp[1]);
            if(parseInt(rsp[1]) == 0) {
                // alert("参数文件已产生");
                // ParameterButton.disabled = false;
                ParameterButton.style.backgroundColor = "";
            }
        }
        else if(rsp[0]=="DataProcessorNTask"){
            DPNTaskSpan.value = parseInt(rsp[1]);
            if(DPNTaskSpan.value == 0){
                DPNTaskSpan.style.backgroundColor = "red";
            }else if(DPNTaskSpan.value > 0){
                DPNTaskSpan.style.backgroundColor = "blue";
            }else{
                DPNTaskSpan.style.backgroundColor = "gray";
            }
        }
        else if(rsp[0]=="DataProcessorIsSaveRawEvent"){
            if(parseInt(rsp[1]) == 0){
                RawEventStartButton.style.backgroundColor = '';
                RawEventStopButton.style.backgroundColor = '';
            }else if(parseInt(rsp[1]) == 1){
                RawEventStartButton.style.backgroundColor = '#00FF00';
                RawEventStopButton.style.backgroundColor = '';
            }
        }
        else if(rsp[0]=="DataProcessorIsSaveEvent"){
            if(parseInt(rsp[1]) == 0){
                EventStartButton.style.backgroundColor = '';
                EventStopButton.style.backgroundColor = '';
            }else if(parseInt(rsp[1]) == 1){
                EventStartButton.style.backgroundColor = '#00FF00';
                EventStopButton.style.backgroundColor = '';
            }
        }
        else if(rsp[0]=="DataProcessorCurrentFileID"){
            outputFileIDSpan.value = parseInt(rsp[1]);
            if(outputFileIDSpan.value > 0){
                outputFileIDSpan.style.backgroundColor = "blue";
            }else{
                outputFileIDSpan.style.backgroundColor = "gray";
            }
        }
        else if(rsp[0]=="DataProcessorState"){
            DPState = parseInt(rsp[1]);
            updateGUI();
        }
    }
    function updateGUI(){
        //DPConnectionButton
        if(DPState == -2){
            DPConnectionButton.style.backgroundColor = "yellow";
            DPConnectionButton.disabled = false;
        }else if(DPState > -2){
            DPConnectionButton.style.backgroundColor = "#00FF00";
            DPConnectionButton.disabled = true;
        }
        //DPInitButton
        if(DPState == -2){
            DPInitButton.style.backgroundColor = "";
            DPInitButton.disabled = true;
        }else if(DPState == -1){
            DPInitButton.style.backgroundColor = "yellow";
            DPInitButton.disabled = false;
        }else if(DPState > -1){
            DPInitButton.style.backgroundColor = "#00FF00";
            DPInitButton.disabled = true;
        }
        //DPShutdownButton
        if(DPState < 0){
            DPShutdownButton.style.backgroundColor = "";
            DPShutdownButton.disabled = true;
        }else if(DPState == 0 ){
            DPShutdownButton.style.backgroundColor = "";
            DPShutdownButton.disabled = false;
        }else if(DPState == 2){
            DPShutdownButton.style.backgroundColor = "";
            DPShutdownButton.disabled = true;
        }else if(DPState == 4){
            DPShutdownButton.style.backgroundColor = "yellow";
            DPShutdownButton.disabled = false;
        }
        //DPStartButton
        if(DPState < 0){
            DPStartButton.style.backgroundColor = "";
            DPStartButton.disabled = true;
        }else if(DPState == 0){
            DPStartButton.style.backgroundColor = "yellow";
            DPStartButton.disabled = false;
        }else if(DPState == 2){
            DPStartButton.style.backgroundColor = "#00FF00";
            DPStartButton.disabled = true;
        }else if(DPState > 2){
            DPStartButton.style.backgroundColor = "";
            DPStartButton.disabled = true;
        }
        //DPStopButton
        if(DPState <= 0){
            DPStopButton.style.backgroundColor = "";
            DPStopButton.disabled = true;
        }else if(DPState == 2){
            DPStopButton.style.backgroundColor = "";
            DPStopButton.disabled = false;
        }else if(DPState == 4){
            DPStopButton.style.backgroundColor = "red";
            DPStopButton.disabled = true;
        }
        //other buttons
        if(DPState == 0 || DPState == 2){
            ParameterButton.disabled = false;
            RawEventStartButton.disabled = false;
            RawEventStopButton.disabled = false;
            EventStartButton.disabled = false;
            EventStopButton.disabled = false;
        }else{
            ParameterButton.disabled = true;
            ParameterButton.style.backgroundColor = "";
            RawEventStartButton.disabled = true;
            RawEventStartButton.style.backgroundColor = "";
            RawEventStopButton.disabled = true;
            RawEventStopButton.style.backgroundColor = "";
            EventStartButton.disabled = true;
            EventStartButton.style.backgroundColor = "";
            EventStopButton.disabled = true;
            EventStopButton.style.backgroundColor = "";
        }
    }
    function dump(){
        // wsSend('dumpDP');
        wsSend('getDataProcessorState');
        wsSend('getDataProcessorTotalEvent');
        wsSend('getDataProcessorCurrentEventID');
        wsSend('getDataProcessorCurrentFileID');
        wsSend('getDataProcessorParameterEvents');
        wsSend('getDataProcessorNTask');
        wsSend('getDataProcessorIsSaveRawEvent');
        wsSend('getDataProcessorIsSaveEvent');
    }
    // connect();
    setInterval(dump, 1000);
    var DPEventNumber = 0;
    function estimateEventRate(){
        DPCurrentEventNumber=parseInt(DPTotalEventSpan.value);
        Interval = parseInt(DPRateIntervalInput.value);
        if(DPEventNumber>0){
            rate = (DPCurrentEventNumber-DPEventNumber)/Interval;
            DPEventRateSpan.value = parseFloat(rate).toFixed(2);
            if(rate == 0){
                DPEventRateSpan.style.backgroundColor = "red";
            }else{
                DPEventRateSpan.style.backgroundColor = "blue";
            }
        }
        DPEventNumber= DPCurrentEventNumber;
        DPRateIntervalButton.disabled = false;
        DPRateIntervalButton.style.backgroundColor = "";
    }
    DPRateIntervalID = setInterval(estimateEventRate, 10000);
    init()
    updateGUI();
})(document);


((doc) => {
    //===========================QAProcessor部分===========================
    // QAProcessor的按钮并禁用
    const Buttons = doc.querySelectorAll('#QAProcessor button');
    Buttons.forEach((button) => {
        button.disabled = true;
    });
    const ConnectionButton = doc.querySelector('#QA_connect');
    const InitButton = doc.querySelector('#QA_init');
    const ShutdownButton = doc.querySelector('#QA_shutdown')
    const StartButton = doc.querySelector('#QA_start');
    const StopButton = doc.querySelector('#QA_stop');
    const QATotalEventSpan = doc.querySelector('#QATotalEvent');
    QATotalEventSpan.value = '';
    const QACurrentEventIDSpan = doc.querySelector('#QACurrentEventID');
    QACurrentEventIDSpan.value = '';
    const QAEventRateSpan = doc.querySelector('#QAEventRate');
    QAEventRateSpan.value = '';
    const hostAddressInput = doc.querySelector('#Host_IP');
    const hostPortInput = doc.querySelector('#Host_Port');
    const HttpServerPort = document.querySelector('#HttpServerPort');
    HttpServerPort.value = '8008';
    const ClearPlotsButton = doc.querySelector('#QA_ClearPlots');
    const QAEventParametersInput = doc.querySelector('#QA_EventParameter');
    QAEventParametersInput.value = './output/eventParameters.json'
    const QARateIntervalInput = doc.querySelector('#QA_RateIntervalInput');
    QARateIntervalInput.value = '10';
    const QARateIntervalButton = doc.querySelector('#QA_RateInterval');
    const QAMemoryUsageSpan = doc.querySelector('#MemoryUsage');
    QAMemoryUsageSpan.value = '';

    QAState = -2;

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
        QARateIntervalID = setInterval(estimateEventRate, parseInt(QARateIntervalInput.value)*1000);
        QARateIntervalButton.disabled = true;
        QARateIntervalButton.style.backgroundColor = "gray";
    }
    function ClearPlots(e){
        wsSend('onlineQAClearPlots');
    }

    function QAProcessorShutdown(e){
        wsSend('shutdownOnlineQA');
        ShutdownButton.disabled = true;
        ShutdownButton.style.backgroundColor='gray';
    }
    function QAProcessorStop(){
        wsSend('stopOnlineQA');
        StopButton.disabled = true;
        StopButton.backgroundColor = "gray";
    }
    function QAProcessorStart(){
        wsSend('startOnlineQA');
        setTimeout(reloadQAFram, 1000);
        StartButton.disabled = true;
        StartButton.backgroundColor = "gray";
    }
    function reloadQAFram(){
        var iframe = doc.querySelector('#QAFrame');
        iframe.src = 'http://'+hostAddressInput.value+':'+HttpServerPort.value;
    }
    function QAProcessorInit(e){
        const command = `initOnlineQA ${HttpServerPort.value} ${QAEventParametersInput.value}`;
        wsSend(command)
        InitButton.disabled = true;
        InitButton.style.backgroundColor = 'gray';
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
        QAState = -1;
        updateGUI();
        wsSend('register')
    }
    function handleClose (e) {
        connection = false;
        QAState = -2;
        delete ws;
        ws = null;
        updateGUI();
    }
    function handleError (e) {
        connection = false;
        QAState = -2;
        delete ws;
        ws = null;
        updateGUI();
    }
    function handleMessage (e) {
	    // console.log('message', e);
        var rsp = e.data.split(" ");
        if(rsp[0]=="OnlineQATotalEvent"){
            QATotalEventSpan.value = parseInt(rsp[1]);
            if(QATotalEventSpan.value == 0){
                QATotalEventSpan.style.backgroundColor = "red";
            }else if(QATotalEventSpan.value > 0){
                QATotalEventSpan.style.backgroundColor = "blue";
            }else{
                QATotalEventSpan.style.backgroundColor = "gray";
            }
        }
        else if(rsp[0]=="OnlineQACurrentEventID"){
            QACurrentEventIDSpan.value = parseInt(rsp[1]);
            if(QACurrentEventIDSpan.value == 0){
                QACurrentEventIDSpan.style.backgroundColor = "red";
            }else if(QACurrentEventIDSpan.value > 0){
                QACurrentEventIDSpan.style.backgroundColor = "blue";
            }else{
                QACurrentEventIDSpan.style.backgroundColor = "gray";
            }
        }
        else if(rsp[0]=="ResourceInfo"){
            QAMemoryUsageSpan.value = parseInt(rsp[3]);
            if(parseInt(rsp[3])>90){
                QAMemoryUsageSpan.style.backgroundColor = "red";
            }else if(parseInt(rsp[3])>70){
                QAMemoryUsageSpan.style.backgroundColor = "yellow";
            }else{
                QAMemoryUsageSpan.style.backgroundColor = "blue";
            }
        }
        else if(rsp[0] == "OnlineQAState"){
            QAState = parseInt(rsp[1]);
            updateGUI();
        }
    }
    function updateGUI(){
        //QAConnectionButton
        if(QAState == -2){
            ConnectionButton.style.backgroundColor = "yellow";
            ConnectionButton.disabled = false;
        }else if(QAState > -2){
            ConnectionButton.style.backgroundColor = "#00FF00";
            ConnectionButton.disabled = true;
        }
        //QAInitButton
        if(QAState == -2){
            InitButton.style.backgroundColor = "";
            InitButton.disabled = true;
        }else if(QAState == -1){
            InitButton.style.backgroundColor = "yellow";
            InitButton.disabled = false;
        }else if(QAState > -1){
            InitButton.style.backgroundColor = "#00FF00";
            InitButton.disabled = true;
        }
        //QAShutdownButton
        if(QAState < 0){
            ShutdownButton.style.backgroundColor = "";
            ShutdownButton.disabled = true;
        }else if(QAState == 0 ){
            ShutdownButton.style.backgroundColor = "";
            ShutdownButton.disabled = false;
        }else if(QAState == 2){
            ShutdownButton.style.backgroundColor = "";
            ShutdownButton.disabled = true;
        }else if(QAState == 4){
            ShutdownButton.style.backgroundColor = "yellow";
            ShutdownButton.disabled = false;
        }
        //QAStartButton
        if(QAState < 0){
            StartButton.style.backgroundColor = "";
            StartButton.disabled = true;
        }else if(QAState == 0){
            StartButton.style.backgroundColor = "yellow";
            StartButton.disabled = false;
        }else if(QAState == 2){
            StartButton.style.backgroundColor = "#00FF00";
            StartButton.disabled = true;
        }else if(QAState > 2){
            StartButton.style.backgroundColor = "";
            StartButton.disabled = true;
        }
        //QAStopButton
        if(QAState <= 0){
            StopButton.style.backgroundColor = "";
            StopButton.disabled = true;
        }else if(QAState == 2){
            StopButton.style.backgroundColor = "";
            StopButton.disabled = false;
        }else if(QAState == 4){
            StopButton.style.backgroundColor = "red";
            StopButton.disabled = true;
        }
        //other buttons
        if(QAState == 0 || QAState == 2){
            ClearPlotsButton.disabled = false;
        }else{
            ClearPlotsButton.disabled = true;
            ClearPlotsButton.style.backgroundColor = "";
        }
    }
    function dump(){
        wsSend('getOnlineQAState');
        wsSend('getOnlineQATotalEvent');
        wsSend('getOnlineQACurrentEventID')
        wsSend('getResourceInfo')
    }
    setInterval(dump, 1000);

    var QAEventNumber = 0;
    function estimateEventRate(){
        QACurrentEventNumber=parseInt(QATotalEventSpan.value);
        Interval = parseInt(QARateIntervalInput.value);
        if(QAEventNumber>0){
            rate = (QACurrentEventNumber-QAEventNumber)/Interval;
            QAEventRateSpan.value = parseFloat(rate).toFixed(2);
            if(rate == 0){
                QAEventRateSpan.style.backgroundColor = "red";
            }else{
                QAEventRateSpan.style.backgroundColor = "blue";
            }
        }
        QAEventNumber= QACurrentEventNumber;
        QARateIntervalButton.disabled = false;
        QARateIntervalButton.style.backgroundColor = "";
    }
    QARateIntervalID = setInterval(estimateEventRate, 10000);
    init()
    updateGUI();
})(document);

((doc) => {
    //===========================Log部分===========================
    const connectionButton = doc.querySelector('#log_connect');
    connectionButton.style.backgroundColor = "yellow";
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
        connectionButton.style.backgroundColor = 'yellow';
        connectionButton.disabled = false;
        createLogButton.style.backgroundColor = '';
        createLogButton.disabled = true;
        delete ws;
        ws = null;
    }
    function handleError (e) {
        connection = false;
        connectionButton.style.backgroundColor = 'yellow';
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
