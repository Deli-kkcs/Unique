using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyCube : MonoBehaviour
{
    [SerializeField][Header("可以存在的最低高度")]
    private float lowestY = -20f;
    private void Update()
    {
        if(GetComponent<LineRenderer>())
            RefreshLine();
        if(transform.position.y <= lowestY)
            Destroy(gameObject);
    }
    void RefreshLine()
    {
        LineRenderer line = GetComponent<LineRenderer>();
        line.SetPosition(0,transform.position);
        line.SetPosition(1, new(transform.position.x, -3, transform.position.z));
    }
}
