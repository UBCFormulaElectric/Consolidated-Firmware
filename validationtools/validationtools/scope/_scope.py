import datetime
import pyvisa as visa
import time
import numpy as np
from tqdm import tqdm
import pandas as pd


class Scope:
    def __init__(self):
        '''
        Represents a Rigol DS1054Z oscilloscope. You can write commands to the scope as well as collect data
        from it as both screenshots and data points.
        '''
        resources = visa.ResourceManager()

        resourceTag = ''
        for x in resources.list_resources():
            print(x)
            if (x[23:24] == 'S'): resourceTag = x

        if (resourceTag == ''):
            print("No scope detected.")
            return

        self.scope = resources.open_resource(resourceTag)

    def stop(self):
        self.scope.write(":STOP")

    def run(self):
        self.scope.write(":RUN")

    def single(self):
        self.scope.write(':SING')

    def channel_on(self, channel):
        """
        Turns on the given channel.

        channel: integer 1 to 4
        """
        self.scope.write(f':CHANnel{channel}:DISPlay ON')

    def channel_off(self, channel):
        """
        Turns off the given channel.

        channel: integer 1 to 4
        """
        self.scope.write(f'CHANnel{channel}:DISPlay OFF')

    def only_channel(self, channel):
        """
        Turns on the given channel and turns off all of the other channels.

        channel: integer 1 to 4
        """
        self.channel_on(channel)
        for x in range(1, 5):
            if (x != channel):
                self.channel_off(x)

    def all_channels(self):
        """
        Turns on all channels.
        """
        for x in range(1, 5):
            self.channel_on(x)

    def offset(self, channel, value):
        """
        Sets the DC offset of the given channel.

        channel: integer 1 to 4
        value: offset value in volts 
        """
        self.scope.write(f':CHANnel{channel}:OFFSet {value}')

    def screenshot(self, filename=''):
        '''
        Saves a screen capture of the scope to the working directory as a png file.
        
        filename: if filename is not provided, a filename with the date and time will be generated.
        '''
        self.scope.write(':DISP:DATA? ON,OFF,PNG')
        raw_data = self.scope.read_raw()[11:]  # strip off first 11 bytes
        # save image file
        if (filename == ''):
            filename = "rigol_" + datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S") + ".png"
        elif (filename[len(filename) - 4:] != ".png"):
            filename += ".png"
        fid = open(filename, 'wb')
        fid.write(raw_data)
        fid.close()
        print("Wrote screen capture to filename " + '\"' + filename + '\"')
        time.sleep(5)

    def vertical_scale(self, channel, scale):
        '''
        sets the vertical scale of the channel in volts
        
        channel: integer 1 to 4
        
        scale: 10mV to 100V
        '''
        self.scope.write(f':CHANnel{channel}:SCALe {scale}')

    def vertical_scale_all(self, scale):
        '''
        scales all channels by the given scaling factor
        
        scale: 0mV to 100V
        '''
        for x in range(1, 5):
            self.vertical_scale(x, scale)

    def horizontal_scale(self, scale):
        """
        sets the horizontal
        
        scale: real value in seconds per division
        """
        self.scope.write(f':TIMebase:MAIN:SCALe {scale}')

    def write(self, message):
        '''
        Writes commands directly to the scope. Only use if you require a command that is not supported here.
        
        If there is a missing feature you would like supported, please let the developer know. 
        
        message: command to be given to the scope
        '''
        self.scope.write(message)

    def __data_collector(self, channel):
        '''
        Private method for handling data collection.
        
        channel: integer 1 to 4
        
        returns: an array of timestamps and voltage values
        '''
        self.channel_on(channel)
        self.scope.write(':stop')
        self.scope.write(':wav:sour chan%i' % channel)
        self.scope.write(':wav:mode norm')
        self.scope.write(':wav:form byte')

        info = self.get_data_premable()

        max_num_pts = 250000
        num_blocks = info['points'] // max_num_pts
        last_block_pts = info['points'] % max_num_pts

        datas = []
        for i in tqdm(range(num_blocks + 1), ncols=60):
            if i < num_blocks:
                self.scope.write(':wav:star %i' % (1 + i * 250000))
                self.scope.write(':wav:stop %i' % (250000 * (i + 1)))
            else:
                if last_block_pts:
                    self.scope.write(':wav:star %i' % (1 + num_blocks * 250000))
                    self.scope.write(':wav:stop %i' % (num_blocks * 250000 + last_block_pts))
                else:
                    break
            self.scope.write(':wav:data?')
            time.sleep(0.3)
            data = self.scope.read_raw()[11:]

            data = np.frombuffer(data, 'B')
            datas.append(data)

        datas = np.concatenate(datas)
        v = (datas - info['yorigin'] - info['yreference']) * info['yincrement']
        t = np.arange(0, info['points'] * info['xincrement'], info['xincrement'])

        cutoff = min(t.size, v.size)
        t = t[:cutoff]
        v = v[:cutoff]
        return [t, v]

    def get_data(self, channel, filename=''):
        '''
        Get oscilloscope data points saved as a csv file. File will be saved to your working directory.
        
        channel: Integer 1 to 4
        
        filename: if filename is not provided, a filename with the date and time will be generated.
        '''
        data = self.__data_collector(channel)

        df = pd.DataFrame({"time": data[0], "voltage": data[1]})
        if (filename == ''):
            filename = "rigol_waveform_data_channel_" + str(channel) + "_" + datetime.datetime.now().strftime(
                "%Y-%m-%d_%H-%M-%S") + ".csv"
        elif (filename[len(filename) - 4:] != ".csv"):
            filename += ".csv"

        df.to_csv(filename, index=False)

    def get_all_channels(self, filename=''):
        '''
        Collect data points for all channels. File will be saved to your working directory.
        
        filename: if filename is not provided, a filename with the date and time will be generated.
        '''

        data1 = self.__data_collector(1)
        t = data1[0]

        ch1 = data1[1]

        ch2 = self.__data_collector(2)[1]

        ch3 = self.__data_collector(3)[1]

        ch4 = self.__data_collector(4)[1]

        df = pd.DataFrame({"time": t, "Channel 1": ch1, "Channel 2": ch2, "Channel 3": ch3, "Channel 4": ch4})
        if (filename == ''):
            filename = "rigol_waveform_all_channels_" + datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S") + ".csv"
        elif (filename[len(filename) - 4:] != ".csv"):
            filename += ".csv"

        df.to_csv(filename)

    def get_data_premable(self):
        '''
        Get information about oscilloscope axes.

        Returns:
            dict: A dictionary containing general oscilloscope axes information.
        '''
        pre = self.scope.query(':wav:pre?').split(',')
        pre_dict = {
            'format': int(pre[0]),
            'type': int(pre[1]),
            'points': int(pre[2]),
            'count': int(pre[3]),
            'xincrement': float(pre[4]),
            'xorigin': float(pre[5]),
            'xreference': float(pre[6]),
            'yincrement': float(pre[7]),
            'yorigin': float(pre[8]),
            'yreference': float(pre[9]),
        }
        return pre_dict
