import os
import argparse

LOG4J_PROPERTIES = '''\
###############################################################################
# This file is auto-generated. DO NOT MODIFY!
###############################################################################
# This sets the global logging level and specifies the appenders
log4j.rootLogger=ERROR, theConsoleAppender

# settings for the console appender
log4j.appender.theConsoleAppender=org.apache.log4j.ConsoleAppender
log4j.appender.theConsoleAppender.layout=org.apache.log4j.PatternLayout
log4j.appender.theConsoleAppender.layout.ConversionPattern=%-4r [%t] %-5p %c %x - %m%n
'''

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--log4j_properties_output', help='Path to output file storing log4j properties')
    args = parser.parse_args()

    log4j_properties = args.log4j_properties_output
    log4j_properties_dir = os.path.dirname(log4j_properties)
    if not os.path.exists(log4j_properties_dir):
        os.makedirs(log4j_properties_dir)

    # Generate a config file to make STM32CubeMX logging output less verbose
    log4j_properties_f = open(log4j_properties, 'w')
    log4j_properties_f.write(LOG4J_PROPERTIES)
    log4j_properties_f.close()
