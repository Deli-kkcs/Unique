using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;

public class CameraController : MonoBehaviour
{
    public enum MouseState
    {
        None,
        Move,
    }
    private MouseState mMouseState = MouseState.None;
    private Camera mCamera;

    private int index_god = 0;

    [Header("��������Text")]
    public Text text_index_cude;
    [Header("ʱ������Text")]
    public Text text_timeScale;
    [Header("�ϵ�ģʽText")]
    public Text text_GodType;
    private int index_cude = 0;
    private int count_index_cube = 3;
    [Header("��ʵ����")]
    public List<GameObject> list_myCube;
    [Header("����Ԥ��")]
    public GameObject fancyCube;
    Vector3 mouse;//�����Ļ����
    Vector3 startDrag;
    GameObject dragTarget;
    bool isDrag = false;
    private void Awake()
    {
        mCamera = GetComponent<Camera>();
        if (mCamera == null)
        {
            Debug.LogError(GetType() + "camera Get Error ����");
        }
        
        mouse.z = 10;//Ĭ��zֵ���������֮��ľ���
        GetDefaultFov();
    }
    private void Update()
    {
        RefreshUI();
    }
    private void LateUpdate()
    {
        InputTime();
        if (Time.timeScale == 0f)
            return;
        CameraRotate();

        CameraFOV();

        CameraMove();

        if(index_god == 0)
        {
            InputCreate();
            InputDestroy();
        }
            
        else if(index_god == 1)
        {
            InputDrag();
        }

        InputGod();

        
    }


    #region RefreshUI
    void RefreshUI()
    {
        switch(index_cude)
        {
            case 0:
                text_index_cude.text = "��ͨ����";
                break;
            case 1:
                text_index_cude.text = "��������";
                break;
            case 2:
                text_index_cude.text = "ը��";
                break;
            default:
                break;
        }
        if (Time.timeScale == 0f)
            text_timeScale.text = "PAUSED";
        else if (Time.timeScale == 1f)
            text_timeScale.text = "x1";
        else if (Time.timeScale == 2f)
            text_timeScale.text = "x2";
        if (index_god == 0)
            text_GodType.text = "����ģʽ";
        else if(index_god == 1)
            text_GodType.text = "��קģʽ";
    }
    #endregion

    #region Input Time
    void InputTime()
    {
        if (Input.GetKeyDown(KeyCode.Keypad0))
            Time.timeScale = 0f;
        else if (Input.GetKeyDown(KeyCode.Keypad1))
            Time.timeScale = 1f;
        else if (Input.GetKeyDown(KeyCode.Keypad2))
            Time.timeScale = 2f;
    }
    #endregion

    #region Camera Rotation
    [Header("��ת�Ƕ���ֵ")]
    public int yRotationMinLimit = -20;
    public int yRotationMaxLimit = 80;
    [Header("��ת�ٶ�")]
    public float xRotationSpeed = 250.0f;
    public float yRotationSpeed = 120.0f;
    //��ת�Ƕ�
    private float xRotation = 0.0f;
    private float yRotation = 0.0f;
    void CameraRotate()
    {
        if (Input.GetMouseButton(1))
        {
            //Input.GetAxis("MouseX")��ȡ����ƶ���X��ľ���
            xRotation -= Input.GetAxis("Mouse X") * xRotationSpeed * 0.02f;
            yRotation += Input.GetAxis("Mouse Y") * yRotationSpeed * 0.02f;

            yRotation = ClampValue(yRotation, yRotationMinLimit, yRotationMaxLimit);//��������ڽ�β
                                                                                    //ŷ����ת��Ϊ��Ԫ��
            Quaternion rotation = Quaternion.Euler(-yRotation, -xRotation, 0);
            transform.rotation = rotation;
        }
    }
    #endregion

    #region Camera fov
    [Header("�ӽ���ֵ")]
    public int fovMinLimit = 25;
    public int fovMaxLimit = 75;
    [Header("�ӽǱ仯�ٶ�")]
    public float fovSpeed = 50.0f;
    //�ӽǽǶ�
    private float fov = 0.0f;
    void GetDefaultFov()
    {
        fov = mCamera.fieldOfView;
    }
    public void CameraFOV()
    {
        //��ȡ�����ֵĻ�����
        fov += -Input.GetAxis("Mouse ScrollWheel") * Time.deltaTime * 100 * fovSpeed;
        // fov ��������
        fov = ClampValue(fov, fovMinLimit, fovMaxLimit);
        //�ı������ fov
        mCamera.fieldOfView = (fov);
    }
    #endregion

    #region Camera Move
    float _mouseX = 0;
    float _mouseY = 0;
    [Header("���ƽ���ٶ�")]
    public float moveSpeed = 1;
    public void CameraMove()
    {
        _mouseX = Input.GetAxis("Horizontal");
        _mouseY = Input.GetAxis("Vertical");
        Vector3 moveDir = (_mouseX * transform.right + _mouseY * transform.forward);
        moveDir.y = 0;
        transform.position += moveDir * 0.5f * moveSpeed;
    }
    #endregion

    #region Input Create
    [Header("������ȸı��ٶ�")]
    public float depthSpeed = 0.2f;
    [SerializeField][Header("���������ֵ")]
    float min_depth = 5f;
    [SerializeField]
    float max_depth = 20f;

    void InputCreate()
    {
        list_myCube[count_index_cube].SetActive(true);
        if(Input.GetKeyDown(KeyCode.Tab))
        {
            index_cude++;
            if (index_cude == count_index_cube)
                index_cude = 0;
        }
        mouse = new(Input.mousePosition.x, Input.mousePosition.y,mouse.z);
        if (Input.GetKey(KeyCode.E) && mouse.z >= min_depth)
            mouse.z -= depthSpeed * Time.deltaTime;
        else if (Input.GetKey(KeyCode.Q) && mouse.z <= max_depth)
            mouse.z += depthSpeed * Time.deltaTime;
        Vector3 worldmouse = Camera.main.ScreenToWorldPoint(mouse);//��Ļ����������
        list_myCube[count_index_cube].transform.position = worldmouse;//Ԥ��λ��
        if (Input.GetMouseButtonDown(0))//������
        {
            GameObject a = Instantiate(list_myCube[index_cude]);
            a.transform.position = worldmouse;//��������=�����������
            a.SetActive(true);
        }
    }
    #endregion

    #region Input Destroy
    void InputDestroy()
    {
        if (Input.GetMouseButtonDown(1))
        {
            Ray camRay = Camera.main.ScreenPointToRay(Input.mousePosition);
            if (Physics.Raycast(camRay, out RaycastHit hits))
            {
                if (hits.transform.gameObject.CompareTag("Cube"))
                    Destroy(hits.transform.gameObject);
            }
        }
    }
    #endregion

    #region Input Drag
    void InputDrag()
    {
        if(!isDrag)
        {
            if(Input.GetMouseButtonDown(0))
            {
                Ray camRay = Camera.main.ScreenPointToRay(Input.mousePosition);
                if (Physics.Raycast(camRay, out RaycastHit hits))
                {
                    if (hits.transform.gameObject.CompareTag("Cube"))
                    {
                        dragTarget = hits.transform.gameObject;
                        startDrag = hits.transform.position;
                        isDrag = true;
                    }
                }
            }
        }
        else
        {
            if (Input.GetMouseButtonUp(0))
            {
                Vector3 mouse = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y,
                Camera.main.transform.position.z > 0 ? Camera.main.transform.position.z : -Camera.main.transform.position.z));
                Vector3 delta = mouse - startDrag;
                //delta.y = 0;
                Debug.Log("Drag Successfully ! mouse: " + mouse + " delta = " + delta);
                dragTarget.GetComponent<Rigidbody>().AddForce(delta*100f);
                isDrag = false;
            }
        }
    }
    #endregion

    #region Input God
    void InputGod()
    {
        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            index_god = 0;
            list_myCube[count_index_cube].SetActive(true);
        }
            
        if (Input.GetKeyDown(KeyCode.Alpha2))
        {
            isDrag = false;
            index_god = 1;
            list_myCube[count_index_cube].SetActive(false);
        }
        
    }
    #endregion

    

    #region ClampValue
    //ֵ��Χֵ�޶�
    float ClampValue(float value, float min, float max)//������ת�ĽǶ�
    {
        if (value < -360)
            value += 360;
        if (value > 360)
            value -= 360;
        return Mathf.Clamp(value, min, max);//����value��ֵ��min��max֮�䣬 ���valueС��min������min�� ���value����max������max�����򷵻�value
    }
    #endregion
}