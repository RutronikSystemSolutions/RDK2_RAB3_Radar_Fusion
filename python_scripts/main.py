# Rutronik Elektronische Bauelemente GmbH Disclaimer: The evaluation board
# including the software is for testing purposes only and,
# because it has limited functions and limited resilience, is not suitable
# for permanent use under real conditions. If the evaluation board is
# nevertheless used under real conditions, this is done at one’s responsibility;
# any liability of Rutronik is insofar excluded

import numpy as np
import json
import radarprocessor
import statistics
import arrayutils
import matplotlib.pyplot as plt

# Load radar measurement values
def load_from_directory(directory_path):
    file_path = directory_path + "\\RadarIfxAvian_00\\radar.npy"
    radar_data = np.load(file_path)
    return radar_data

# Load radar configuration
def load_config_from_directory(directory_path):
    file_path = directory_path + "\\RadarIfxAvian_00\\config.json"
    f = open(file_path)
    config = json.load(f)
    f.close()
    return config

if __name__ == '__main__':
    radar_data = load_from_directory("data")
    radar_config = load_config_from_directory("data")

    # Check if at least 2 antennas
    if (len(radar_config['device_config']['fmcw_single_shape']['rx_antennas']) < 2):
        print("Need 2 RX antennas at least...")
        exit(0)

    processor = radarprocessor.RadarProcessor()
    processor.set_data_and_config(radar_data, radar_config)
    frameCount = len(radar_data)
    antennaIndex = 0
    rangeFFTLen = processor.get_range_fft_len()
    chirpsPerFrame = processor.get_chirps_per_frame()
    chirpRepetitionTime = processor.get_chirps_repetition_time()
    frameRepetitionTime = processor.get_frame_repetition_time()
    maxrange = processor.get_max_range()

    celerity = 299792458
    groundfreq = 60000000000
    lambdaval = celerity / groundfreq

    vmin = -lambdaval / (4 * chirpRepetitionTime)
    vmax = lambdaval / (4 * chirpRepetitionTime)
    vres = (vmax - vmin) / chirpsPerFrame

    cmap = plt.get_cmap("viridis")
    cmap.set_under("black")

    # List of frames for which you want to display the Doppler FFT
    observedFrames = [268]

    maxAmplitudeOverTime = []

    # For each frame compute Doppler FFT
    for frameIndex in range(frameCount):
        # First get the range FFT (for every chirp within the frame)
        # Size of the array [chirps per frame][fft len]
        # with fft len = (samples per chirp / 2) + +1
        frameRangeFFT = processor.get_frame_range_fft(frameIndex, antennaIndex)

        dopplerFFTValues = []

        # For each bin (or range/distance)
        for binIndex in range(rangeFFTLen):
            binContent = []

            # Extract the complex value over the chirps
            for chirpIndex in range(chirpsPerFrame):
                binContent.append(frameRangeFFT[chirpIndex][binIndex])

            # Compute average
            avgComplex = statistics.avg(binContent)
            binFormatted = arrayutils.apply_offset(binContent, -avgComplex)

            # Compute doppler FFT (one per bin)
            fftout = np.fft.fftshift(np.fft.fft(binFormatted))
            dopplerFFTValues.append(fftout)

        # Get max amplitude
        maxValue = arrayutils.get_matrix_max(np.abs(dopplerFFTValues))
        maxAmplitudeOverTime.append(maxValue)

        if (observedFrames.__contains__(frameIndex)):
            plt.figure()
            im = plt.imshow(np.abs(dopplerFFTValues), cmap=cmap, aspect='auto', interpolation='nearest',
                            extent=[vmin, vmax, maxrange, 0])
            plt.xlabel("Speed (m/s)")
            plt.ylabel("Range (meters)")
            plt.title("Doppler FFT - Frame at time" + "{0:0.0f} seconds".format(frameIndex * frameRepetitionTime))
            plt.colorbar(im)
            plt.grid(True)

    # Plot the amplitude over the time
    plt.figure()
    plt.plot(maxAmplitudeOverTime)
    plt.grid(True)
    plt.xlabel("Frame index")
    plt.ylabel("Amplitude")
    plt.title("Doppler FFT amplitude over time")

    plt.show()
