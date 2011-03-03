# R file to load a Layer's debug output and generate a PDF from it
#
# Author: Nathan Merritt
# Date: March 2011

Args <- commandArgs(TRUE)
inputFile <- Args[1]
outputFile <- Args[2]

if(!file.exists(inputFile))
	quit("no")
dat = read.table(inputFile,header=T,dec=".",na.strings=c("-"))

pdf(outputFile,width=10, height=4)
plot(dat$Timestep,dat$Activation,pch="",main="",xlab="time",ylab="activation")
names = c("AverageActivity")
cols = c("1")

points(dat$Timestep,dat$AverageActivity,type="l",col=1)

legend("top",lwd=2,legend=names,col=cols)
dev.off()
