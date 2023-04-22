import os
import shutil
import urllib.request

audio_urls = {
    'https://www.chosic.com/wp-content/uploads/2021/09/Feel-Good.mp3',
}

audio_dir = 'resources/audio/'

if __name__ == '__main__':
    if not os.path.exists(audio_dir):
        os.mkdir(audio_dir)

    # download audio
    for url in audio_urls:
        filename = url.split('/')[-1]

        local_path = os.path.join(audio_dir, filename)
        print('Downloading ' + filename)

        request = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
        with urllib.request.urlopen(request) as response, open(local_path, 'wb') as out_file:
            shutil.copyfileobj(response, out_file)
