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
    job = {1: [jobNum, status, pagesReq, jobMem, runTime, currTime, startTime, endTime]}
    totTime = runTime
    print("{0: >7}{1: >11}{2: >10}".format((jobNum), runTime, jobMem))
    for i in range(2, numJobs + 1):
        newSeed+=1
        random.seed(newSeed)
        jobMem = random.randint(memMin, memMax)
        runTime = random.randint(runTimeMin, runTimeMax)
        pagesReq = (jobMem // pageSize)
        if (jobMem % pageSize) != 0:
            pagesReq += 1
        jobNum = i
        job[i] = [jobNum, status, pagesReq, jobMem, runTime, currTime, startTime, endTime]

        print("{0: >7}{1: >11}{2: >10}".format((jobNum), runTime, jobMem))
        totTime = totTime + runTime
    return totTime, job

def simulateCalc(totTime, numPages, runTime, jobMem, numJobs):
    print("\nSimulator Starting:")
    i = 1
    j = 1
    endTime = []
    while i <= totTime:
        print("\nTime Step {0}: ".format(i))
        if i == 1:
            for num in range(numJobs):
                print("  Job {0} is starting".format(num + 1))
                endTime.append(-1)
        # Decide which job is running
        job = (j%3)
        if job == 0:
            job = 3
        # If a job is completed, the end time will no longer be -1
        # This indicates that said job may be skipped
        while endTime[job-1] > -1:
            job+=1
            job=job%3
            if job == 0:
                job = 3
            j+=1

        print("  Job {0} Running".format(job))
        # Decrements job time by 1 and when it hits 0, the completion time is recorded
        runTime[job-1] = runTime[job-1] - 1
        if runTime[job-1] == 0:
            print("  Job {0} Completed".format(job))
            endTime[job-1] = i
        print("  Page Table:")
        i+=1
        j+=1
    # Final start and end times for each job
    print("\nJob Information:\n  Job #    Start Time    End Time")
    for jobs in range(numJobs):
        print("{0: >7}{1: >14}{2: >12}".format((jobs + 1), '1', endTime[jobs]))


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
    pagesAvail = totNumPages

    print("\nSimulator Parameters:\n  Memory Size: {0}\n  Page Size: {1}\n  Random Seed: {2}\n"
          "  Number of Jobs: {3}\n  Runtime (min-max) Timesteps: {4}-{5}\n"
          "  Memory (min-max): {6}-{7}\n".format(memSize, pageSize, seed, numJobs,
                                                     runTimeMin, runTimeMax, memMin, memMax))

    totTime, job = jobCreations(pageSize, runTimeMin, runTimeMax, memMin, memMax, randSeed, numJobs)
    #simulateCalc(totTime, numPages, runTime, jobMem, numJobs)

main()