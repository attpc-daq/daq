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
    // 设置主机地址的默认值
    const hostAddressInput = doc.querySelector('#Host_IP');
    hostAddressInput.value = '10.155.101.208';
    // 设置端口的默认值
    const hostPortInput = doc.querySelector('#Host_Port');
    hostPortInput.value = '8000';
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
    const hostAddressInput = doc.querySelector('#Host_IP');
    const hostPortInput = doc.querySelector('#Host_Port');
    const HttpServerPort = document.querySelector('#HttpServerPort');
    HttpServerPort.value = '8008';
    const storageDir = doc.querySelector('#QADataDir');
    storageDir.value='./output';
    const QAPadRow = doc.querySelector('#PadRow');
    QAPadRow.value = 15;
    const QAPadColumn = doc.querySelector('#PadColumn');
    QAPadColumn.value = 31;
    const ClearPlotsButton = doc.querySelector('#QA_ClearPlots');
    const FirstFileButton = doc.querySelector('#QA_FirstFile');
    const PreviousFileButton= doc.querySelector('#QA_PreviousFile');
    const FileIDInput = doc.querySelector('#QA_FileID');
    const NextFileButton = doc.querySelector('#QA_NextFile');
    const LastFileButton = doc.querySelector('#QA_LastFile');
    const AutoFileButton = doc.querySelector('#QA_AutoFile');

    const FirstEventButton = doc.querySelector('#QA_FirstEvent');
    const PreviousEventButton= doc.querySelector('#QA_PreviousEvent');
    const EventEntryIDInput = doc.querySelector('#QA_EventEntryID');
    const NextEventButton = doc.querySelector('#QA_NextEvent');
    const LastEventButton = doc.querySelector('#QA_LastEvent');
    //const AutoEventButton = doc.querySelector('#QA_AutoEvent');

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

        storageDir.addEventListener('keydown',SetDir,false);

        QAPadRow.addEventListener('keydown',SetQAPad,false);
        QAPadColumn.addEventListener('keydown',SetQAPad,false);

        ClearPlotsButton.addEventListener('click',ClearPlots,false);

        FirstFileButton.addEventListener('click',DoFirstFile,false);
        PreviousFileButton.addEventListener('click',DoPreviousFile,false);
        FileIDInput.addEventListener('keydown',SetFileID,false);
        NextFileButton.addEventListener('click',DoNextFile,false);
        LastFileButton.addEventListener('click',DoLastFile,false);
        AutoFileButton.addEventListener('click',AutoFile,false);

        FirstEventButton.addEventListener('click',DoFirstEvent,false);
        PreviousEventButton.addEventListener('click',DoPreviousEvent,false);
        EventEntryIDInput.addEventListener('keydown',SetEventEntryID,false);
        NextEventButton.addEventListener('click',DoNextEvent,false);
        LastEventButton.addEventListener('click',DoLastEvent,false);
        //AutoEventButton.addEventListener('click',AutoEvent,false);
    }
    function SetDir(e){
        wsSend('setQADir '+storageDir.value);
    }
    function SetQAPad(e){
        if(e.key==='Enter'){
            const command = `setQAPad ${QAPadRow.value} ${QAPadColumn.value}`;
            wsSend(command);
        }
    }
    function ClearPlots(e){
        wsSend('QAClearPlots');
    }
    function DoFirstFile(e){
        wsSend('QADoFirstFile');
    }
    function DoPreviousFile(e){
        wsSend('QADoPreviousFile');
    }
    function SetFileID(e){
        wsSend('QADoFile '+FileIDInput.value);
    }
    function DoNextFile(e){
        wsSend('QADoNextFile');
    }
    function DoLastFile(e){
        wsSend('QADoLastFile');
    }
    function AutoFile(e){
        wsSend('QASetAutoFile');
    }

    function DoFirstEvent(e){
        wsSend('QADoFirstEvent');
    }
    function DoPreviousEvent(e){
        wsSend('QADoPreviousEvent');
    }
    function SetEventEntryID(e){
        wsSend('QADoEvent '+EventEntryIDInput.value);
    }
    function DoNextEvent(e){
        wsSend('QADoNextEvent');
    }
    function DoLastEvent(e){
        wsSend('QADoLastEvent');
    }
    //function AutoEvent(e){
    //    wsSend('QASetAutoEvent');
    //}

    function QAProcessorShutdown(e){
        wsSend('shutdownQA');
    }
    function QAProcessorStop(){
        wsSend('stopQA');
    }
    function QAProcessorStart(){
        wsSend('startQA');
        setTimeout(reloadQAFram, 2000);
    }
    function reloadQAFram(){
        var iframe = doc.querySelector('#QAFrame');
        iframe.src = 'http://'+hostAddressInput.value+':'+HttpServerPort.value;
    }
    function QAProcessorInit(e){
        const command = `initQA ${HttpServerPort.value} ${storageDir.value} ${QAPadRow.value} ${QAPadColumn.value}`;
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
        if(rsp[0]=="QATotalEvent"){
            QATotalEventSpan.innerText = parseInt(rsp[1]);
        }
        else if(rsp[0]=="QAFileID"){
            FileIDInput.value = parseInt(rsp[1]);
        }
        else if(rsp[0]=="QAAutoFileMode"){
            if(rsp[1] == "-1"){
                AutoFileButton.style.backgroundColor='yellow';
            }else if(rsp[1] == "False"){
                AutoFileButton.style.backgroundColor='';
            }else if(rsp[1]=="True"){
                AutoFileButton.style.backgroundColor='#00FF00';
            }
        }
        //else if(rsp[0]=="QAAutoEventMode"){
        //    if(rsp[1] == "-1"){
        //        AutoEventButton.style.backgroundColor='yellow';
        //    }else if(rsp[1] == "False"){
        //        AutoEventButton.style.backgroundColor='';
        //    }else if(rsp[1]=="True"){
        //        AutoEventButton.style.backgroundColor='#00FF00';
        //    }
        //}
        else if(rsp[0]=="QACurrentEventID"){
            QACurrentEventIDSpan.innerText = parseInt(rsp[1]);
        }
        else if(rsp[0]=="QAEventEntryID"){
            EventEntryIDInput.value = parseInt(rsp[1]);
        }
        else if(rsp[0] == "QAState"){
            if(parseInt(rsp[1]) == -1){
                InitButton.style.backgroundColor = 'yellow';
		ShutdownButton.style.backgroundColor = '';
                StartButton.style.backgroundColor = '';
                StopButton.style.backgroundColor = '';

                // ClearPlotsButton.disabled = true;
                // FirstButton.disabled = true;
                // PreviousButton.disabled = true;
                // NextButton.disabled = true;
                // LastButton.disabled = true;

                // AutoButton.disabled = true;

                InitButton.disabled = false;
		ShutdownButton.disabled = true;
                StartButton.disabled = true;
                StopButton.disabled = true;
            }else if(parseInt(rsp[1]) == 0){
                InitButton.style.backgroundColor = '#00FF00';
		ShutdownButton.style.backgroundColor = '';
                StartButton.style.backgroundColor = 'yellow';
                StopButton.style.backgroundColor = '';
                
                // ClearPlotsButton.disabled = true;
                // FirstButton.disabled = true;
                // PreviousButton.disabled = true;
                // NextButton.disabled = true;
                // LastButton.disabled = true;

                // AutoButton.disabled = false;
                InitButton.disabled = true;
		        ShutdownButton.disabled = false;
                StartButton.disabled = false;
                StopButton.disabled = true;
            }else if(parseInt(rsp[1]) == 2){
                InitButton.style.backgroundColor = '#00FF00';
		ShutdownButton.style.backgroundColor = '';
                StartButton.style.backgroundColor = '#00FF00';
                StopButton.style.backgroundColor = '';

                // LastButton.style.backgroundColor = '';
                // ClearPlotsButton.disabled = false;
                // FirstButton.disabled = false;
                // PreviousButton.disabled = false;
                // NextButton.disabled = false;
                // LastButton.disabled = false;

                // AutoButton.disabled = false;
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
                
                // ClearPlotsButton.disabled = true;
                // FirstButton.disabled = true;
                // PreviousButton.disabled = true;
                // NextButton.disabled = true;
                // LastButton.disabled = true;

                // AutoButton.disabled = true;
                InitButton.disabled = true;
		        ShutdownButton.disabled = false;
                StartButton.disabled = true;
                StopButton.disabled = true;
            }
        }
    }
    function dump(){
        // wsSend('dumpQA');
        wsSend('getQAState');
        wsSend('getQATotalEvent');
        wsSend('getQACurrentEventID');
        wsSend('getQAFileID');
        wsSend('getQAEventEntryID');
        wsSend('getQAAutoFileMode');
        //wsSend('getQAAutoEventMode');
    }
    setInterval(dump, 1000);

    var QAEventNumber = 0;

    init()

})(document);