
df <- read.csv ("~/Documents/code/school/cosc5324/Project/uwcProc_driver-features.csv", header = TRUE)
labels <- read.csv ("~/Documents/code/school/cosc5324/Project/labels")
labels$LabelsA = as.character(labels$LabelsA)
labels$LabelsB = as.character(labels$LabelsB)
rownames(df) <- df[,1]
df[,1] <- NULL

plot (df$SobelMean)
plot (df$SobelSum)
plot (df$LaplacianMean)
plot (df$LaplacianSum)
plot (df$HueMean)
plot (df$HueMax)
plot (df$HueMin)
plot (df$HueRange)


#df$SobelMean <- NULL
#df$SobelSum <- NULL
#df$LaplacianMean <- NULL
#df$LaplacianSum <- NULL
#df$HueMean <- NULL
#df$HueMax <- NULL
#df$HueMin <- NULL
#df$HueRange <- NULL



mydata <- df
wss <- (nrow(mydata)-1)*sum(apply(mydata,2,var))
for (i in 2:15) wss[i] <- sum(kmeans(mydata,
                                     centers=i)$withinss)
plot(1:15, wss, type="b", xlab="Number of Clusters",
     ylab="Within groups sum of squares") 

# K-Means Cluster Analysis
fit <- kmeans(mydata, 2) # 5 cluster solution
# get cluster means
aggregate(mydata,by=list(fit$cluster),FUN=mean)
# append cluster assignment
mydata <- data.frame(mydata, fit$cluster, labels) 


mydata[mydata$LabelsB == 1,]$fit.cluster

mydata[mydata$LabelsB == 0,]$fit.cluster






library (rpart)
#fit <- rpart (LabelsA ~ SobelMean + SobelSum + HueMean + HueMax + HueMin, method = "class", data = mydata)
fit <- rpart (LabelsA ~ CannyMean + SobelMean + HueMean + CannySum + HueRange + Entropy, method = "class", data = mydata)
printcp(fit) # display the results
plotcp(fit) # visualize cross-validation results
summary(fit) # detailed summary of splits

# plot tree
plot(fit, uniform=TRUE,
     main="Classification Tree for Seagrass")
text(fit, use.n=TRUE, all=TRUE, cex=.8)

# prune the tree
pfit<- prune(fit, cp=   fit$cptable[which.min(fit$cptable[,"xerror"]),"CP"])

# plot the pruned tree
plot(pfit, uniform=TRUE,
     main="Pruned Classification Tree for Seagrass")
text(pfit, use.n=TRUE, all=TRUE, cex=.8)


predicted <- predict (fit, mydata, type = "class")
results <- data.frame(predicted, mydata$LabelsA)

percentCorrect <- length (which (results$predicted == results$mydata.LabelsA)) / nrow (results)
print (percentCorrect)


