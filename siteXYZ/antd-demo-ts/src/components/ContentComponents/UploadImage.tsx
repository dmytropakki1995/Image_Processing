import React, { useState, useContext } from 'react';
import { Upload, Icon, message, Select, Button } from 'antd';
import { useDispatch } from 'react-redux';
import { setImageProcessingType, imageLoaded } from '../../service/action';
import '../../styles/uploadImage.css'
import { ThemeContext } from '../../App';
import { RcFile } from 'antd/lib/upload';
const { Option } = Select;

const getBase64 = (img: any, callback: any) => {
    const reader = new FileReader();
    reader.addEventListener('load', () => callback(reader.result));
    reader.readAsDataURL(img);
}

const beforeUpload = (file: any) => {
    const isJpgOrPng = file.type === 'image/jpeg' || file.type === 'image/png';
    if (!isJpgOrPng) {
        message.error('You can only upload JPG/PNG file!');
    }
    const isLt2M = file.size / 1024 / 1024 < 2;
    if (!isLt2M) {
        message.error('Image must smaller than 2MB!');
    }
    return isJpgOrPng && isLt2M;
}

const UploadImage: React.FC = () => {
    const dispatch = useDispatch();
    console.log(useContext(ThemeContext));
    const theme = useContext(ThemeContext);
    console.log(theme)
    const imgCallback = (imgUrl: any) => {
        setImageUrl(imgUrl)
        setIsLoading(false)
    }
    const [isLoading, setIsLoading] = useState(false);
    const [imageUrl, setImageUrl] = useState<string | undefined>(undefined);
    const handleChange = (info: any) => {
        if (info.file.status === 'uploading') {
            setIsLoading(true);
            return;
        }
        if (info.file.status === 'done') {
            // Get this url from response in real world.
            getBase64(info.file.originFileObj, imgCallback);
        }
    };

    const uploadButton = (
        <div>
            <Icon type={isLoading ? 'loading' : 'plus'} />
            <div className="ant-upload-text">Upload</div>
        </div>
    );

    return (
        <div className='uploadImage'>
            <Select defaultValue={0} style={{ width: 150 }} onChange={(value: number) => {dispatch(setImageProcessingType(value))}}>
                <Option value={10001} >Gradient</Option>
                <Option value={10002}>Laplasian</Option>
                <Option value={0} disabled>
                    Choose Type
                </Option>
                <Option value={10003}>PCY</Option>
            </Select>
            <Upload
                name="avatar"
                listType="picture-card"
                className="avatar-uploader, uploadImage"
                showUploadList={false}
                action={ (file: RcFile) => { dispatch(imageLoaded(file)); return ' ';} }
                beforeUpload={beforeUpload}
                onChange={handleChange}
                style={{right: 25}}
            >
                {imageUrl ? <img src={imageUrl} alt="avatar" style={{ width: '100%' }} /> : uploadButton}
            </Upload>
            <Button >
                Press to apply filter
            </Button>
            <p> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.</p>
            <p> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.</p>
            <p> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.</p>
            <p> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.</p>
        
        </div>
    );
}


export default UploadImage;