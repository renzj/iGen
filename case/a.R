SecPerPoint = 5 

QpsFromFile <- function(filePath){
    data <- read.table(filePath, header = TRUE)
    # group by 1000
    data$Time = floor(data$Time / 1000)
    Value = tapply(data$Value, data$Time, sum)
    Value = tapply(Value, rep(1:ceiling(length(Value) / SecPerPoint), each = SecPerPoint, length.out = length(Value)), mean)
    Time = unique(data$Time)
    Time = Time - Time[1]
    Time = tapply(Time, rep(1:ceiling(length(Time) / SecPerPoint), each = SecPerPoint, length.out = length(Time)), mean)
    data <- data.frame("Time" = Time, "Value" = Value)
    return(data)
}
data <- QpsFromFile("period_Execute_qps.log")
print(data)
