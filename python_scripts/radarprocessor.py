# Rutronik Elektronische Bauelemente GmbH Disclaimer: The evaluation board
# including the software is for testing purposes only and,
# because it has limited functions and limited resilience, is not suitable
# for permanent use under real conditions. If the evaluation board is
# nevertheless used under real conditions, this is done at one’s responsibility;
# any liability of Rutronik is insofar excluded

import numpy as np
import arrayutils
import statistics
from scipy import signal

class RadarProcessor:

    def __init__(self):
        self.radar_data = []
        self.radar_config = []

    def set_data_and_config(self, data, config):
        self.radar_data = data
        self.radar_config = config

    # Get FFT of every chirp in the frame
    # Results is an array of dimension [chirps per frame][fft len] containing complex numbers
    def get_frame_range_fft(self, frame_index, antenna_index):
        # Extract some relevant information
        chirps_per_frame_count = len(self.radar_data[0][antenna_index])

        frame_range_fft = []

        for chirp_index in range(chirps_per_frame_count):
            # Get the time signal of the chirp
            timesignal = self.radar_data[frame_index][antenna_index][chirp_index]

            # Get the range FFT
            rangefft = self.get_range_fft(timesignal)

            frame_range_fft.append(rangefft)

        return frame_range_fft

    # From a time signal, get the Range FFT
    # Output is an array containing complex values of size len(timesignal/2) + 1
    def get_range_fft(self, timesignal):
        sample_count = len(timesignal)
        # Compute window
        window = self.get_window(sample_count)
        # Scale and shift
        timesignal = arrayutils.multiply(timesignal, 1 / 4095)
        avgValue = statistics.avg(timesignal)
        shiftedSignal = arrayutils.apply_offset(timesignal, -avgValue)

        for sindex in range(sample_count):
            shiftedSignal[sindex] = window[sindex] * shiftedSignal[sindex]

        # Get FFT output
        return np.fft.rfft(shiftedSignal)

    def get_window(self, sample_count):
        # Blackmann-harris window
        window = signal.blackmanharris(sample_count)
        return window

    def get_range_fft_len(self):
        sample_count = int(self.radar_config['device_config']['fmcw_single_shape']['num_samples_per_chirp'])
        return int((sample_count/2) + 1)

    def get_chirps_per_frame(self):
        return int(self.radar_config['device_config']['fmcw_single_shape']['num_chirps_per_frame'])

    def get_chirps_repetition_time(self):
        return (self.radar_config['device_config']['fmcw_single_shape']['chirp_repetition_time_s'])

    def get_frame_repetition_time(self):
        return (self.radar_config['device_config']['fmcw_single_shape']['frame_repetition_time_s'])

    def get_max_range(self):
        end_frequency = float(self.radar_config['device_config']['fmcw_single_shape']['end_frequency_Hz'])
        start_frequency = float(self.radar_config['device_config']['fmcw_single_shape']['start_frequency_Hz'])
        sampling_rate = float(self.radar_config['device_config']['fmcw_single_shape']['sample_rate_Hz'])
        sample_count = float(self.radar_config['device_config']['fmcw_single_shape']['num_samples_per_chirp'])

        bandwidth = end_frequency - start_frequency
        # c * f / (2 * Delta(f) / Delta(t))
        # Delta(f) -> Bandwidth
        # Delta(t) -> chirp duration
        slope = bandwidth / (sample_count * (1 / sampling_rate))

        c = 299792458
        maxrange = (c * sampling_rate / 2) / (2 * slope)
        return maxrange
