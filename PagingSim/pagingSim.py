import sys
import os
import random

def validInput(args):
    # A method to check that the proper number of variables were given
    if len(args) != 8:
        print("Incorrect Input:")
        print("python3 pagingSim.py <memory size> <page size> <number of jobs> <runtime min> "
              "<runtime max> <memory min> <memory max>")
        sys.exit(1)

def checkSeed(randSeed):
    # A method to check that the random seed has been set and chooses random seed if not
    if randSeed == None:
        randSeed = random.randint(1,100)
    return randSeed

def validPageSize(memSize, pageSize):
    # A method to check that the memory size is an even multiple of page size
    if (memSize%pageSize) != 0:
        print("Page Size Must Be An Even Multiple Of Memory Size")
        sys.exit(1)

def jobCreations(pageSize, runTimeMin, runTimeMax, memMin, memMax, randSeed, numJobs):
    # A method to create job run times and memory and print them
    print("Job Queue:\n  Job #    Runtime    Memory")
    newSeed = int(randSeed)
    random.seed(newSeed)
    currentTime = 0
    # status: -1 = not started, 0 = running, 1 = completed
    status = -1
    startTime = -1
    endTime = -1
    currTime = 0
    jobMem = random.randint(memMin,memMax)
    runTime = random.randint(runTimeMin, runTimeMax)
    pagesReq = (jobMem//pageSize)
    if (jobMem%pageSize) != 0:
        pagesReq+=1
    jobNum = 1
    job = {0: [jobNum, status, pagesReq, jobMem, runTime, currTime, startTime, endTime]}
    totTime = runTime
    print("{0: >7}{1: >11}{2: >10}".format((jobNum), runTime, jobMem))
    for i in range(1, numJobs):
        newSeed+=1
        random.seed(newSeed)
        jobMem = random.randint(memMin, memMax)
        runTime = random.randint(runTimeMin, runTimeMax)
        pagesReq = (jobMem // pageSize)
        if (jobMem % pageSize) != 0:
            pagesReq += 1
        jobNum = i + 1
        job[i] = [jobNum, status, pagesReq, jobMem, runTime, currTime, startTime, endTime]

        print("{0: >7}{1: >11}{2: >10}".format((jobNum), runTime, jobMem))
        totTime = totTime + runTime
    return totTime, job

def loadJob(job, totPages, jobsRunning):
    pagesA = totPages
    for run in range(len(jobsRunning)):
        pagesA = pagesA - job[jobsRunning[run] - 1][2]
    for num in job:
        if job[num][1] == -1 and job[num][2] <= pagesA and job[num][0] not in jobsRunning:
            jobsRunning.append(job[num][0])
            pagesA = pagesA - job[num][2]
    return jobsRunning

def removeJob(job, jobsRunning, currJob, i):
    if job[jobsRunning[currJob] - 1][5] == job[jobsRunning[currJob] - 1][4]:
        print("  Job {0} Completed".format(jobsRunning[currJob]))
        job[jobsRunning[currJob] - 1][1] = 1
        job[jobsRunning[currJob] - 1][7] = i
        job[jobsRunning[currJob] - 1][2] = 0
    return jobsRunning

def pageTable(job, jobsRunning, pagesAvail):
    pageTable = []
    for i in range(len(jobsRunning)):
        for j in range(job[jobsRunning[i] - 1][2]):
            pageTable.append(str(jobsRunning[i]))
    pages = int(pagesAvail) - (len(pageTable))
    for page in range(pages):
        pageTable.append('.')
    pageTableStr = "  "
    j = 1
    for i in range(len(pageTable)):
        pageTableStr = pageTableStr + pageTable[i]
        if j%4 == 0:
            pageTableStr = pageTableStr + ' '
        if j%16 == 0:
            pageTableStr = pageTableStr + '\n  '
        j+=1
    print("  Page Table:")
    print(pageTableStr)



def simulateCalc(job, totTime, totNumPages, numJobs):
    # job[i] = [jobNum, status, pagesReq, jobMem, runTime, currTime, startTime, endTime]
    pagesAvail = totNumPages
    jobsRunning = []
    print("\nSimulator Starting:")
    i = 1
    k = 0
    bool = False
    # run for total time for all jobs
    while i <= totTime:
        print("\nTime Step {0}: ".format(i))
        jobsRunning = loadJob(job, pagesAvail, jobsRunning)
        # Prints the jobs that are starting
        for j in range(len(jobsRunning)):
            if job[jobsRunning[j]-1][1] == -1:
                print("  Job {0} is starting".format(jobsRunning[j]))
                job[jobsRunning[j]-1][1] = 0
                job[jobsRunning[j]-1][6] = i


        currJob = k%(len(jobsRunning))
        while job[jobsRunning[currJob] - 1][1] != 0:
            k+=1
            currJob = k%(len(jobsRunning))
        print("  Job {0} is Running".format(jobsRunning[currJob]))
        job[jobsRunning[currJob] - 1][5] += 1
        jobsRunning = removeJob(job, jobsRunning, currJob, i)

        i+=1
        k+=1
        pageTable(job, jobsRunning, totNumPages)



    print("\nJob Information:\n  Job #    Start Time    End Time")
    for jobs in range(numJobs):
        print("{0: >7}{1: >14}{2: >12}".format(job[jobs][0], job[jobs][6], job[jobs][7]))



def main():
    # Get arguments from command line
    validInput(sys.argv)
    memSize = int(sys.argv[1])
    pageSize = int(sys.argv[2])
    validPageSize(memSize, pageSize)
    # 'export RANDOM_SEED=decimal' in command line
    randSeed = os.environ.get("RANDOM_SEED")
    seed = checkSeed(randSeed)
    numJobs = int(sys.argv[3])
    runTimeMin = int(sys.argv[4])
    runTimeMax = int(sys.argv[5])
    memMin = int(sys.argv[6])
    memMax = int(sys.argv[7])
    totNumPages = memSize/pageSize


    print("In Main {0}".format(totNumPages))

    print("\nSimulator Parameters:\n  Memory Size: {0}\n  Page Size: {1}\n  Random Seed: {2}\n"
          "  Number of Jobs: {3}\n  Runtime (min-max) Timesteps: {4}-{5}\n"
          "  Memory (min-max): {6}-{7}\n".format(memSize, pageSize, seed, numJobs,
                                                     runTimeMin, runTimeMax, memMin, memMax))

    totTime, job = jobCreations(pageSize, runTimeMin, runTimeMax, memMin, memMax, seed, numJobs)

    simulateCalc(job, totTime, totNumPages, numJobs)


main()