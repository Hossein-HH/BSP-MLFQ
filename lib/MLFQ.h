//
// Created by Huseyn on 1/21/2022.
//

#ifndef MLFQ_MLFQ_H
#define MLFQ_MLFQ_H

#include "iostream"
#include "fstream"
#include "string"
#include "queue"
#include "vector"
#include "algorithm"
//!#include "map"
#include "list"

using namespace std;

string input;

int Dispatch_Latency,
        RR_Quantum;

struct processBound {
    int boundTime;
    string boundType;
};

struct process {
    int id;
    int arrivalTime;
//    int terminate = -1;
    queue<processBound> processBounds;

    //? HRRN parameters
    double responseRatio;
    int timeToLineUp; // the time when processes go to the HRRN queue

    //? IO parameter
    int WhenCurrentIoExpires;

    //? avgs parameters
    bool alreadyGotACpu = false;
    int FirstCpu;
    int sumBursts;


};

struct finishedProcess {
    int id;
    int arrivalTime;
    int terminate;
    int responseTime;
    int sumBursts;
    int waitingTime;
};

struct GantProcesses {
    int id;
    int time;
};

queue<string> initializeTurn;
list<int> processesId;
list<int> processesArrival;
vector<process> processes;
vector<finishedProcess> finishedProcesses;
vector<GantProcesses> GantChart;
//!map<int, int> id_arrival;
queue<process> RoundRobin;
vector<process> HRRN;
vector<process> IO;

//? parameters for calculate CPU utilization , avg s
int usefulOperatingTime = 0;
int totalTime;

bool isWhiteSpace(char character);

int ignoreWhiteSpaces(int currIndex);

string buffering(int currIndex);

//!void sort(map<int, int> &id_arr);

void sortProcessesByArrivalTime();

void initializeProcessResponseRatio(int totalTime);

void sortProcessesByResponseRatio();

//bool cmp(pair<int, int> &a,
//         pair<int, int> &b);

void writeFile();


void calculateProcessWhenCurrentIoExpires();

void sortProcessByWhenCurrentIoExpires();

int responseTime();

int turnAroundTime();

int waitingTime();

void calculateSumBursts();

void fetchData() {
    initializeTurn.push("Dispatch_Latency");
    initializeTurn.push("RR_Quantum");
    initializeTurn.push("Process");
}

void readFile() {
    string tmpLine;

    ifstream MyReadFile(R"(F:\CPP\MLFQ\assets\input.txt)");

    while (getline(MyReadFile, tmpLine)) {
        input += tmpLine;
        input += "\n";
    }

    MyReadFile.close();

}

void initialize() {
    fetchData();
    cout << "=============" << endl;
    for (int index = 0; index < input.length(); ++index) {
        index = ignoreWhiteSpaces(index);

        if (initializeTurn.front() == "Dispatch_Latency") {
            Dispatch_Latency = stoi(buffering(index));
            cout << "Dispatch_Latency : " << Dispatch_Latency << endl;
            initializeTurn.pop();

        } else if (initializeTurn.front() == "RR_Quantum") {
            RR_Quantum = stoi(buffering(index));
            cout << "RR_Quantum : " << RR_Quantum << endl;
            cout << "=============" << endl;
            initializeTurn.pop();

        } else if (initializeTurn.front() == "Process") {
            string processBuffer = buffering(index);
//            cout << processBuffer << endl;

            string tmpBuffer;
            int tmpBufferIndex;
            process tmpProcess;
            processBound tmpProcessBound;
            int numOfCama = 0;

            for (tmpBufferIndex = 1; processBuffer[tmpBufferIndex] != ':'; ++tmpBufferIndex)
                tmpBuffer += processBuffer[tmpBufferIndex];


            tmpProcess.id = stoi(tmpBuffer);
            cout << "processId : " << tmpBuffer << endl;

            tmpBuffer = "";

            for (++tmpBufferIndex; processBuffer[tmpBufferIndex] != ','; ++tmpBufferIndex)
                tmpBuffer += processBuffer[tmpBufferIndex];

            tmpProcess.arrivalTime = stoi(tmpBuffer);
            cout << "arrival time : " << tmpBuffer << endl;
            tmpBuffer = "";

            //? we count the ',' and we can know from num of them => curr type burst
            numOfCama++;

            cout << "bounds : ";
            while (tmpBufferIndex < processBuffer.length()) {
                for (++tmpBufferIndex; processBuffer[tmpBufferIndex] != ','
                                       && tmpBufferIndex < processBuffer.length(); ++tmpBufferIndex)
                    tmpBuffer += processBuffer[tmpBufferIndex];

                tmpProcessBound.boundTime = stoi(tmpBuffer);

                /// numOfCama % 2 == 1 ~~ cpu Burst
                /// numOfCama % 2 == 0 ~~ I/O Burst

                (numOfCama % 2 == 1) ? tmpProcessBound.boundType = "CPU"
                                     : tmpProcessBound.boundType = "IO";

                tmpProcess.processBounds.push(tmpProcessBound);

                cout << tmpProcess.processBounds.back().boundTime;
                (tmpBufferIndex + 1 < processBuffer.length()) ? cout << " , "
                                                              : cout << "";

                tmpBuffer = "";
                //? we count the ',' and we can know from num of them => curr type burst
                numOfCama++;
            }
            cout << endl << "=============" << endl;

            processes.push_back(tmpProcess);
            processesId.push_back(tmpProcess.id);
            processesArrival.push_back(tmpProcess.arrivalTime);

//!            //? we define (0 arr) => (-1 arr) because 0 return from map when not founded any value for a key
//!            (tmpProcess.arrivalTime == 0) ?
//!            id_arrival.insert(pair<int, int>(-1, tmpProcess.id)) :
//!            id_arrival.insert(pair<int, int>(tmpProcess.arrivalTime, tmpProcess.id));



            //? move cursor to end line
            index += processBuffer.length();
        }
    }

    //? we sort the processes by arrival time
//!    sort(id_arrival);
    sortProcessesByArrivalTime();
    calculateSumBursts();


}

void calculateSumBursts() {
    int tmpSumBursts = 0;

    for (int i = 0; i < processes.size(); ++i) {
        queue<processBound> tmpProcessBound;
        tmpProcessBound = processes[i].processBounds;

        for (int j = 0; j < tmpProcessBound.size(); ++j) {
            tmpSumBursts += tmpProcessBound.front().boundTime;
            tmpProcessBound.pop();
        }
        processes[i].sumBursts = tmpSumBursts;
    }
}

//! bool cmp(pair<int, int> &a,
//!         pair<int, int> &b) {
//!    return a.first < b.first;
//! }

//!void sort(map<int, int> &id_arr) {
//!
//!    vector<pair<int, int> > sorted_id_arr;
//!
//!    for (auto &it: id_arr) {
//!        sorted_id_arr.push_back(it);
//!    }
//!
//!    sort(sorted_id_arr.begin(), sorted_id_arr.end(), cmp);
//!
//!   for (auto &it: sorted_id_arr) {
//!        cout << it.first << ' '
//!             << it.second << endl;
//!    }
//!}

int ignoreWhiteSpaces(int currIndex) {
    for (; (currIndex < input.length()) && isWhiteSpace(input[currIndex]); ++currIndex);
    return currIndex;
}

string buffering(int currIndex) {
    string buffer;
    for (; (currIndex < input.length()) && !(isWhiteSpace(input[currIndex])); ++currIndex) {
        buffer += input[currIndex];
    }
    return buffer;
}

bool isWhiteSpace(char character) {
    if (character == 9 ||
        character == 10 ||
        character == 32)
        return true;
    return false;
}


//! =======================================================



void MLFQ() {

    int tmpCpuBound = 0;


    vector<process> processesCopy = processes;

    //? for dispatch latency

    bool IsFirstProcess = true;
    bool ZeroHour = false;


    //? simulation of time (each totalTime means 1S)
    for (totalTime = 0;
         ZeroHour ||
         (!processesCopy.empty() && totalTime < processesCopy.back().arrivalTime) ||
         !(RoundRobin.empty()) ||
         !HRRN.empty() ||
         !(IO.empty());) {

        int TTT = totalTime;

        //! 3 if for check process empty or rr or hrrn , ...

        /// adding process to ROUND_ROBIN
        while (!processesCopy.empty() && processesCopy.front().arrivalTime <= totalTime) {
            RoundRobin.push(processesCopy.front());

//            for (auto num: processesCopy)
//                cout << num.id << " , ";
//            cout << endl;

            processesCopy.erase(processesCopy.begin());

        }

        /// run first process in ROUND_ROBIN
        if (!RoundRobin.empty()) {

            //? check is not first processes add dispatch latency
            IsFirstProcess ?
                    IsFirstProcess = false :
                    totalTime += Dispatch_Latency;


            while (!RoundRobin.empty()) {

                cout << "RR selected --->>> p" << RoundRobin.front().id << endl;
//                cout << "front process time : " << RoundRobin.front().processBounds.front().boundTime << endl;
//                cout << "front process type : " << RoundRobin.front().processBounds.front().boundType << endl;

                //? a normal check on processes first bound and type
                if (RoundRobin.front().processBounds.front().boundType == "CPU" &&
                    RoundRobin.front().processBounds.front().boundTime != 0) {

                    ///Gant part1
                    GantProcesses tmpGantProcess;
                    tmpGantProcess.id = RoundRobin.front().id;


                    //? check if first cpu bound of process is lower than RR_Quantum give
                    //? that lower cpu time not RR_Quantum
                    if (RoundRobin.front().processBounds.front().boundTime <= RR_Quantum) {
                        tmpCpuBound = RoundRobin.front().processBounds.front().boundTime;

                        RoundRobin.front().processBounds.pop();



                    } else {
                        tmpCpuBound = RR_Quantum;
                        RoundRobin.front().processBounds.front().boundTime -= RR_Quantum;
                    }

                    //? for Response Time
                    if (!RoundRobin.front().alreadyGotACpu) {
                        RoundRobin.front().alreadyGotACpu = true;
                        RoundRobin.front().FirstCpu = totalTime;
                    }
                    usefulOperatingTime += tmpCpuBound;
                    totalTime += tmpCpuBound;

                    ///Gant part2
                    tmpGantProcess.time = totalTime;
                    GantChart.push_back(tmpGantProcess);

                    //? terminated process
                    if (RoundRobin.front().processBounds.empty()) {

                        finishedProcess tmpFinishedProcess;
                        tmpFinishedProcess.id = RoundRobin.front().id;
                        tmpFinishedProcess.arrivalTime = RoundRobin.front().arrivalTime;
                        tmpFinishedProcess.terminate = totalTime;
                        tmpFinishedProcess.responseTime = RoundRobin.front().FirstCpu - RoundRobin.front().arrivalTime;
                        tmpFinishedProcess.sumBursts = RoundRobin.front().FirstCpu - RoundRobin.front().sumBursts;
///                        totalCpuBurst
///                        &     turnTime

                        RoundRobin.pop();

                        finishedProcesses.push_back(tmpFinishedProcess);

                        //? this else mean the process's cpu burst not completed
                    } else {

                        //? we should push the process when exited from ROUND_ROBIN to IO because (IO , CPU) one by one, and we know first CPU done in ROUND_ROBIN
                        RoundRobin.front().timeToLineUp = totalTime;

                        //? first cpu burst finished or not ?
                        (RoundRobin.front().processBounds.front().boundType == "IO") ?
                        IO.push_back(RoundRobin.front()) :
                        HRRN.push_back(RoundRobin.front());


                        RoundRobin.pop();

//                        cout << "selected --->>> p" << IO.front().id << endl;
//                        cout << "front process time : " << IO.front().processBounds.front().boundTime << endl;
//                        cout << "front process type : " << IO.front().processBounds.front().boundType << endl;
                    }
                }
            }

            //! the bloody hrrn hell
        } else if (RoundRobin.empty() && (!HRRN.empty() || !IO.empty())) {
//            if (HRRN.empty()) {
            calculateProcessWhenCurrentIoExpires();
            sortProcessByWhenCurrentIoExpires();
            /// in this step we have nearest and shortest IOExpire on front of IO Vector

            //? then we check is the first IO Expired ?? if yeh we add them/that to HRRN
            while (!IO.empty() && IO.front().WhenCurrentIoExpires <= totalTime) {
                IO.front().processBounds.pop();
                HRRN.push_back(IO.front());
                IO.erase(IO.begin());
            }

            //? else we do nothing and wait for ++totalTime and com new processes maybe in RR

//            }

            //? we here check Is any process in HRRN after we added totalTime and check IO vector
            if (!HRRN.empty()) {
                initializeProcessResponseRatio(totalTime);
                sortProcessesByResponseRatio();

                usefulOperatingTime += tmpCpuBound;
                totalTime += HRRN.front().processBounds.front().boundTime + Dispatch_Latency;
                cout << "HRRN selected --->>> p" << HRRN.front().id << endl;

                HRRN.front().processBounds.pop();

                //? for Response Time
                if (!HRRN.front().alreadyGotACpu) {
                    HRRN.front().alreadyGotACpu = true;
                    HRRN.front().FirstCpu = totalTime;
                }

                //? if the all of processBounds Finished remove that from ready queue that else back to HRRN with new timeToLineUp
                if (HRRN.front().processBounds.empty()) {

                    GantProcesses tmpGantProcess;
                    tmpGantProcess.id = HRRN.front().id;
                    tmpGantProcess.time = totalTime;
                    GantChart.push_back(tmpGantProcess);

                    finishedProcess tmpFinishedProcess;
                    tmpFinishedProcess.id = HRRN.front().id;
                    tmpFinishedProcess.arrivalTime = HRRN.front().arrivalTime;
                    tmpFinishedProcess.terminate = totalTime;
                    tmpFinishedProcess.responseTime = HRRN.front().FirstCpu - HRRN.front().arrivalTime;
                    tmpFinishedProcess.sumBursts = HRRN.front().sumBursts;

///                        totalCpuBurst
///                        &     turnTime

                    HRRN.erase(HRRN.begin());

                    finishedProcesses.push_back(tmpFinishedProcess);

                } else {
                    HRRN.front().timeToLineUp = totalTime;
                    IO.push_back(HRRN.front());
                    HRRN.erase(HRRN.begin());
                    calculateProcessWhenCurrentIoExpires();

                }
            }
        }

//        cout << endl << "selected --->>> p" << processes[0].id << endl;
//        cout << "front process time : " << processes[0].processBounds.front().boundTime << endl;
//        cout << "front process type : " << processes[0].processBounds.front().boundType << endl;

        //? zero hour is the last check for arrival time (processes coming) // Kabk & barf
        if (!processesCopy.empty())
            (!ZeroHour && totalTime >= processesCopy.back().arrivalTime) ?
                    ZeroHour = true :
                    ZeroHour = false;
        else
            ZeroHour = false;
        //? that if mean's the total time not changed from first to end of loop so we ++ that
        if (TTT == totalTime)
            totalTime++;

    }
    writeFile();
}


void sortProcessesByArrivalTime() {
    int i, j;
    int n = processes.size();
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++)
            if (processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
    }

//    cout << "======================" << endl;
//    for (int k = 0; k < processes.size(); ++k) {
//        cout << "p" << k << " id : " << processes[k].id << endl;
//        cout << "p" << k << " arr : " << processes[k].arrivalTime << endl;
//        cout << "####################" << endl;
//    }

}

void calculateProcessWhenCurrentIoExpires() {
    int start;
    int len;

    for (int i = 0; i < IO.size(); ++i) {
        start = IO[i].timeToLineUp;
        len = IO[i].processBounds.front().boundTime; // we know currently the front of bounds is IO not CPU
        IO[i].WhenCurrentIoExpires = start + len;
    }
}

void sortProcessByWhenCurrentIoExpires() {
    int i, j;
    int n = IO.size();
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++)
            if (IO[j].WhenCurrentIoExpires > IO[j + 1].WhenCurrentIoExpires) {
                process temp = IO[j];
                IO[j] = IO[j + 1];
                IO[j + 1] = temp;
            }
    }

//    cout << "======================" << endl;
//    for (int k = 0; k < IO.size(); ++k) {
//        cout << "p" << k << " id : " << IO[k].id << endl;
//        cout << "p" << k << " expire : " << IO[k].WhenCurrentIoExpires << endl;
//        cout << "####################" << endl;
//    }

}

void initializeProcessResponseRatio(int totalTime) {
    int w, s;

    for (int i = 0; i < HRRN.size(); ++i) {
        w = totalTime - HRRN[i].timeToLineUp;
        s = HRRN[i].processBounds.front().boundTime;
        HRRN[i].responseRatio = double(w) / double(s);
    }

}

void sortProcessesByResponseRatio() {
    int i, j;
    int n = HRRN.size();
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++)
            if (HRRN[j].responseRatio < HRRN[j + 1].responseRatio) {
                process temp = HRRN[j];
                HRRN[j] = HRRN[j + 1];
                HRRN[j + 1] = temp;
            }
    }

//    cout << "======================" << endl;
//    for (int k = 0; k < HRRN.size(); ++k) {
//        cout << "p" << k << " id : " << HRRN[k].id << endl;
//        cout << "p" << k << " res : " << HRRN[k].responseRatio << endl;
//        cout << "####################" << endl;
//    }

}

void writeFile() {
    ofstream MyFile(R"(F:\CPP\MLFQ\assets\output.txt)");

    MyFile
            << "------------------------------------------------------------------------------------------------------------------"
            << endl;

    for (auto num: GantChart)
        MyFile << "|" << "P" << num.id << " ";
//        MyFile << "  | " << "P" << GantChart.front().id;
    MyFile << endl;

    MyFile
            << "------------------------------------------------------------------------------------------------------------------"
            << endl;

    for (auto num: GantChart)
        MyFile << "  " << num.time;
    MyFile << endl;

    //? cpu utilization == usefulOperatingTime / total time
    float cpu_utilization = float(usefulOperatingTime) / (float(totalTime));

    MyFile << "CPU Utilization : " << cpu_utilization * 100 << " %" << endl;
    MyFile << "Average Waiting Time : " << waitingTime() << endl;
    MyFile << "Average Turnaround Time : " << turnAroundTime() << endl;
    MyFile << "Average Response Time : " << responseTime() << endl;

    MyFile.close();
}

int waitingTime() {
    int sumWaitingTime = 0;
    int avgWaitingTime = 0;

    //? calculate waiting time for each process =>> terminated - (sumBursts - arrival)

    for (int i = 0; i < finishedProcesses.size(); ++i) {
        sumWaitingTime +=
                finishedProcesses[i].terminate - (finishedProcesses[i].sumBursts - finishedProcesses[i].arrivalTime);
    }
    avgWaitingTime = sumWaitingTime / finishedProcesses.size();
    return avgWaitingTime;
}


int turnAroundTime() {
    int sumTurnAroundTime = 0;
    int avgTurnAroundTime = 0;

    for (int i = 0; i < finishedProcesses.size(); ++i) {
        sumTurnAroundTime += finishedProcesses[i].terminate - finishedProcesses[i].arrivalTime;
    }
    avgTurnAroundTime = sumTurnAroundTime / finishedProcesses.size();
    return avgTurnAroundTime;
}

int responseTime() {
    int sumResTime = 0;
    int avgResTime = 0;

    for (int i = 0; i < finishedProcesses.size(); ++i) {
        sumResTime += finishedProcesses[i].responseTime;
    }
    avgResTime = sumResTime / finishedProcesses.size();
    return avgResTime;
}


#endif //MLFQ_MLFQ_H